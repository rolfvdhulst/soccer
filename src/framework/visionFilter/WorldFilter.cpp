#include "WorldFilter.h"
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/Camera.h>
#include "BallAssigment.h"

WorldFilter::WorldFilter() {
    robotTrajectories.reserve(32); //An amount which is likely sufficient for entire runtime of the program.
}

void WorldFilter::updateGeometry(const proto::SSL_GeometryData &geometry) {
    geometryData = GeometryData(geometry);
}

proto::World WorldFilter::getWorld(const Time &time) const {
    //TODO: split up in functions for robot and ball
    proto::World world;
    std::vector<FilteredRobot> blueRobots = getHealthiestRobotsMerged(true, time);
    for (const auto &blueBot : blueRobots) {
        world.mutable_blue()->Add()->CopyFrom(blueBot.asWorldRobot());
    }
    std::vector<FilteredRobot> yellowRobots = getHealthiestRobotsMerged(false, time);
    for (const auto &yellowBot : yellowRobots) {
        world.mutable_yellow()->Add()->CopyFrom(yellowBot.asWorldRobot());
    }
    if (!isTrackingVirtualBalls) {
        if (!balls.empty()) {
            double maxHealth = -std::numeric_limits<double>::infinity();
            auto bestFilter = balls.begin();
            for (auto ballFilter = balls.begin(); ballFilter != balls.end(); ++ballFilter) {
                double health = ballFilter->getHealth();
                if (health > maxHealth) {
                    maxHealth = health;
                    bestFilter = ballFilter;
                }
            }
            FilteredBall bestBall = bestFilter->mergeBalls(time);
            world.mutable_ball()->CopyFrom(bestBall.asWorldBall());
        }
    } else {
        auto blueVirtualBalls = virtualBallTracker.getViableVirtualBalls(blueRobots, blueParams);
        for (const auto &blueVirtualBall : blueVirtualBalls) {
            world.mutable_bluevirtual()->Add()->CopyFrom(blueVirtualBall.asProto());
        }
        auto yellowVirtualBalls = virtualBallTracker.getViableVirtualBalls(yellowRobots, yellowParams);
        for (const auto &yellowVirtualBall : yellowVirtualBalls) {
            world.mutable_yellowvirtual()->Add()->CopyFrom(yellowVirtualBall.asProto());
        }
    }
    world.set_time(time.asNanoSeconds());

    return world;
}

void WorldFilter::process(const std::vector<proto::SSL_DetectionFrame> &frames) {
    std::vector<DetectionFrame> detectionFrames;
    for (const auto &protoFrame: frames) {
        detectionFrames.emplace_back(DetectionFrame(protoFrame));
    }
    //Sort by time
    std::sort(detectionFrames.begin(), detectionFrames.end(),
              [](const DetectionFrame &lhs, const DetectionFrame &rhs) { return lhs.timeCaptured < rhs.timeCaptured; });
    for (auto &frame : detectionFrames) {
        auto cameraTime = lastCaptureTimes.find(frame.cameraID);
        frame.dt = cameraTime == lastCaptureTimes.end() ? 0.0 : (cameraTime->second -
                                                                 lastCaptureTimes[frame.cameraID]).asSeconds();
        //TODO: make a realtime option
        //TODO: add moving average filter per camera and see how much it helps?
        //TODO: remove any frames with captures times which differ more than a second from the current time
    }
    //Remove frames which are too late. For now we do this, because it's quite hard to go back in time and reconstruct the state of the entire visionFilter

    //This can also be caused by other teams running e.g. their simulators internally and accidentally broadcasting onto the network
    detectionFrames.erase(std::remove_if(detectionFrames.begin(), detectionFrames.end(),
                                         [](const DetectionFrame &frame) { return frame.dt < 0.0; }),detectionFrames.end());
    for (const auto &frame : detectionFrames) {
        processFrame(frame);
    }
}


void WorldFilter::processRobots(const DetectionFrame &frame, bool blueBots) {
    robotMap &robots = blueBots ? blue : yellow;
    const std::vector<RobotObservation> &detectedRobots = blueBots ? frame.blue : frame.yellow;

    predictRobots(frame, robots);
    updateRobots(blueBots, robots, detectedRobots);
    updateRobotsNotSeen(frame, robots);
}

void WorldFilter::updateRobotsNotSeen(const DetectionFrame &frame, robotMap &robots) {
    for (auto &oneIDFilters : robots) {
        std::vector<RobotFilter> &filters = oneIDFilters.second;
        auto it = filters.begin();
        while (it != filters.end()) {
            if (it->processNotSeen(frame.cameraID, frame.timeCaptured)) {
                it = filters.erase(it);
            } else {
                it++;
            }
        }
    }
}

void WorldFilter::updateRobots(bool blueBots, robotMap &robots,
                               const std::vector<RobotObservation> &detectedRobots) {
    //add detected robots to existing filter(s) or create a new filter if no filter accepts the robot
    for (const auto &detectedRobot : detectedRobots) {
        if (detectedRobot.robotId < 0 || detectedRobot.robotId >= 16) {
            continue;
        }
        std::vector<RobotFilter> &oneIDFilters = robots[detectedRobot.robotId];
        bool accepted = false;
        for (RobotFilter &filter : oneIDFilters) {
            accepted |= filter.processDetection(detectedRobot);
        }
        if (!accepted && oneIDFilters.size() < MAX_ROBOTFILTERS) {
            oneIDFilters.emplace_back(RobotFilter(detectedRobot, blueBots));
        }
    }
}

void WorldFilter::predictRobots(const DetectionFrame &frame, robotMap &robots) {
    for (auto &oneIDFilters : robots) {
        for (auto &filter : oneIDFilters.second) {
            filter.predictCam(frame.cameraID, frame.timeCaptured);
        }
    }
}

void
WorldFilter::processBalls(const DetectionFrame &frame, const std::vector<RobotTrajectorySegment> &robotPaths) {
    std::vector<BallPredictions> predictions;
    for (const auto &filter : balls) {
        predictions.push_back(filter.predictCam(frame.cameraID, frame.timeCaptured, geometryData, robotPaths));
    }
    BallAssignmentResult assignment = assignBalls(predictions,frame.balls);
    // TODO: update all existing ball filters with their assigned balls. Split them if there are matches on multiple branches
    //Create new Ball filters for balls that were not assigned
    for(const auto& newBall : assignment.unpairedBalls){
        balls.emplace_back(BallFilter(newBall));
    }
    // process balls that weren't seen and remove them if necessary
    auto it = balls.begin();
    while (it != balls.end()) {
        bool removeFilter = it->processFrame(frame.cameraID, frame.timeCaptured);
        if (removeFilter) {
            //TODO: make this if there are no more healthy balls. Probably needs a larger refactor and a rethink of how
            // invisible balls are handled
            if (balls.size() == 1 && !isTrackingVirtualBalls) { //TODO; not enable when already tracking virtual balls?
                isTrackingVirtualBalls = true;
                auto lastSeenBall = it->lastDetection();
                std::vector<FilteredRobot> blueBots = getHealthiestRobotsMerged(true, frame.timeCaptured);
                std::vector<FilteredRobot> yellowBots = getHealthiestRobotsMerged(false, frame.timeCaptured);
                blueBots.insert(blueBots.end(), std::make_move_iterator(yellowBots.begin()),
                                std::make_move_iterator(yellowBots.end()));
                virtualBallTracker.generateVirtualBalls(blueBots, lastSeenBall);
            }
            it = balls.erase(it);
        } else {
            it++;
        }
    }
}

void WorldFilter::processFrame(const DetectionFrame &frame) {
    processRobots(frame, true);
    processRobots(frame, false);
    robotTrajectories.clear();
    getPreviousFrameTrajectories(true, frame.cameraID);
    getPreviousFrameTrajectories(false, frame.cameraID);
    processBalls(frame, robotTrajectories);
    processForVirtualBalls(frame);
}

void WorldFilter::getPreviousFrameTrajectories(bool isBlue, int cameraID) {
    const robotMap &robots = isBlue ? blue : yellow;
    const RobotParameters &params = isBlue ? blueParams : yellowParams;
    for (const auto &oneIDFilters : robots) {
        for (const auto &bot : oneIDFilters.second) {
            std::optional<RobotTrajectorySegment> trajectory = bot.getLastFrameTrajectory(cameraID, params);
            if (trajectory) {
                robotTrajectories.emplace_back(*trajectory);
            }
        }
    }
}

void WorldFilter::updateRobotParameters(const proto::TeamRobotInfo &robotInfo) {
    blueParams = RobotParameters(robotInfo.blue());
    yellowParams = RobotParameters(robotInfo.yellow());
}

void WorldFilter::processForVirtualBalls(const DetectionFrame &frame) {
    if (isTrackingVirtualBalls) {
        bool switchBack = false;
        constexpr double healthThreshold = 50.0;
        for (const auto &ballFilter : balls) {
            switchBack |= ballFilter.getHealth() > healthThreshold;
        }
        isTrackingVirtualBalls = !switchBack;
        if (switchBack) {
            virtualBallTracker.clearVirtualBalls();
        } else if (geometryData.cameras.hasCamera(frame.cameraID)) {
            Camera camera = geometryData.cameras.getCamera(frame.cameraID).value();
            std::vector<FilteredRobot> robots = oneCameraHealthyRobots(true, frame.cameraID, frame.timeCaptured);
            virtualBallTracker.updateVirtualBalls(robots, camera, blueParams, frame);
            std::vector<FilteredRobot> yellowBots = oneCameraHealthyRobots(false, frame.cameraID, frame.timeCaptured);
            virtualBallTracker.updateVirtualBalls(yellowBots, camera, yellowParams, frame);
        }
    }
}

std::vector<FilteredRobot> WorldFilter::getHealthiestRobotsMerged(bool blueBots, Time time) const {
    std::vector<FilteredRobot> robots;
    const robotMap &map = blueBots ? blue : yellow;
    for (const auto &oneIDFilters : map) {
        if (oneIDFilters.second.empty()) {
            continue;;
        }
        double maxHealth = -std::numeric_limits<double>::infinity();
        auto bestFilter = oneIDFilters.second.begin();
        for (auto robotFilter = oneIDFilters.second.begin();
             robotFilter != oneIDFilters.second.end(); ++robotFilter) {
            double health = robotFilter->getHealth();
            if (health > maxHealth) {
                maxHealth = health;
                bestFilter = robotFilter;
            }
        }
        robots.push_back(bestFilter->mergeRobots(time));
    }
    return robots;
}

std::vector<FilteredRobot> WorldFilter::oneCameraHealthyRobots(bool blueBots, int camera_id, Time time) const {
    std::vector<FilteredRobot> robots;
    const robotMap &map = blueBots ? blue : yellow;
    for (const auto &oneIDFilters : map) {
        if (oneIDFilters.second.empty()) {
            continue;;
        }
        double maxHealth = -std::numeric_limits<double>::infinity();
        auto bestFilter = oneIDFilters.second.begin();
        for (auto robotFilter = oneIDFilters.second.begin();
             robotFilter != oneIDFilters.second.end(); ++robotFilter) {
            double health = robotFilter->getHealth();
            if (health > maxHealth) {
                maxHealth = health;
                bestFilter = robotFilter;
            }
        }
        auto robot = bestFilter->getRobot(camera_id, time);
        if (robot) {
            robots.push_back(robot.value());
        }
    }
    return robots;
}
