//
// Created by rolf on 15-12-19.
//

#include "CameraFilter.h"

void CameraFilter::process(const DetectionFrame &frame) {
    if(lastDetectionFrameTime<frame.timeCaptured){
        lastDetectionFrameTime = frame.timeCaptured;
    }else{
        std::cerr<<"Delayed? Frame with " <<(lastDetectionFrameTime-frame.timeCaptured).asMilliSeconds()<<" ms not accepted!"<<std::endl;
        return;
    }
    processRobots(frame,true);
    processRobots(frame,false);
    processBalls(frame);
}

void CameraFilter::processRobots(const DetectionFrame &frame, bool blueBots) {
    robotMap& robots = blueBots ? blue : yellow;

    const std::vector<RobotObservation>& detectionRobots = blueBots ? frame.blue: frame.yellow;
    //remove outdate robots
    for(auto& oneIDFilters : robots){
        std::vector<RobotFilter>& filters = oneIDFilters.second;
        auto it = filters.begin();
        for (;it!=filters.end();) {
            if(frame.timeCaptured-it->lastSeen()>Time(0.5)){
                it = filters.erase(it);
            }else{
                ++it;
            }
        }
    }

    //Predict until the frame capture time
    for(auto& oneIDFilters : robots){
        for(auto& filter: oneIDFilters.second){
            filter.predict(frame.timeCaptured);
        }
    }

    //Pass observations to the filters.
    //Note multiple observations of the same object can be made in this frame if they are close enough to the prediction
    for(const RobotObservation& robot : detectionRobots){
        std::vector<RobotFilter>& oneIDFilters = robots[robot.bot.robot_id()];
        bool accepted = false;
        //Attempt to update all filters that track robots with that id.
        for(RobotFilter& filter : oneIDFilters){
            accepted |= filter.update(robot);
        }
        //No existing filters accepted the robot detection so we create a new one with this detection
        if(!accepted){
            oneIDFilters.emplace_back(RobotFilter(robot,blueBots));
        }
    }
    //If a robot is not seen we forward that information to the existing filters with that ID
    for(auto& oneIDFilters : robots){
        for(auto& filter : oneIDFilters.second){
            if(!filter.justUpdated()){
                filter.updateRobotNotSeen(frame.timeCaptured);
            }
        }
    }
}

void CameraFilter::processBalls(const DetectionFrame &frame) {
    //remove filters which have not been seen in a while or that have
    auto it = balls.begin();
    for (;it!=balls.end();) {
        if(frame.timeCaptured-it->lastSeen()>Time(0.5)){
            it = balls.erase(it);
        }else{
            ++it;
        }
    }
    //Predict all existing filters to the frame capture time
    for(auto& ballFilter : balls){
        ballFilter.predict(frame.timeCaptured);
    }
    //Pass observations to the filters and update their error estimates
    //Note multiple observations can be passed. It sometimes happens that the ball is detected as 2 separate
    //balls so this is useful/necessary
    for(const auto & ball : frame.balls){
        bool accepted = false;
        for(BallFilter& ballFilter : balls){
            accepted |= ballFilter.update(ball);
        }
        //If no filters accepted the detected ball we create a new one with this detection
        if(!accepted && balls.size()<MAX_BALLFILTERS){
            balls.emplace_back(BallFilter(ball));
        }
    }
    for(auto& ballFilter : balls){
        if(!ballFilter.justUpdated()){
            ballFilter.updateBallNotSeen(frame.timeCaptured);
        }
    }
}

void CameraFilter::updateCalibration(const Camera &calibration) {
    camera=calibration;
}

void CameraFilter::updateRobotInfo(const proto::TeamRobotInfo& robotInfo) {
    teamInfo = robotInfo;
}

CameraFilter::CameraFilter(const DetectionFrame &frame) {
    process(frame);
}
CameraFilter::CameraFilter(const DetectionFrame &frame, const Camera& calibration) :
camera{calibration}{
    process(frame);
}

std::optional<FilteredBall> CameraFilter::getBestBall(const Time& time) const {
    std::optional<FilteredBall> ball = std::nullopt;
    double currentHealth = -1;
    for (const auto& ballFilter : balls){
        FilteredBall filterBall = ballFilter.getEstimate(time,true);
        if(ballFilter.isHealthy() && filterBall.health > currentHealth){
            ball = filterBall;
            currentHealth = filterBall.health;
        }
    }
    return ball;
}

std::optional<FilteredRobot> CameraFilter::getBestRobot(const Time &time, int id, bool blueBots) const {
    std::optional<FilteredRobot> robot = std::nullopt;
    double currentHealth = -1;
    const robotMap& robots = blueBots ? blue : yellow;
    const auto& iterator = robots.find(id);
    if(iterator!=robots.end() && !iterator->second.empty()){
        for (const auto& robotFilter : iterator->second){
            FilteredRobot filteredRobot = robotFilter.getEstimate(time,true);
            if(robotFilter.isHealthy() && filteredRobot.health > currentHealth){
                robot = filteredRobot;
                currentHealth = filteredRobot.health;
            }
        }
    }
    return robot;
}

std::vector<FilteredBall> CameraFilter::getHealthyBalls(const Time &time) const {
    std::vector<FilteredBall> healthyBalls;
    for (const auto& ballFilter : balls){
        FilteredBall filterBall = ballFilter.getEstimate(time,true);
        if(ballFilter.isHealthy()){
            healthyBalls.emplace_back(filterBall);
        }
    }
    return healthyBalls;
}

std::vector<FilteredRobot> CameraFilter::getHealthyRobots(const Time &time,int id, bool blueBots) const {
    std::vector<FilteredRobot> healthyRobots;
    const robotMap& robots = blueBots ? blue : yellow;
    const auto& iterator = robots.find(id);
    if(iterator!=robots.end() && !iterator->second.empty()){
        for (const auto& robotFilter : iterator->second){
            FilteredRobot filteredRobot = robotFilter.getEstimate(time,true);
            if(robotFilter.isHealthy()){
                healthyRobots.emplace_back(filteredRobot);
            }
        }
    }
    return healthyRobots;
}
