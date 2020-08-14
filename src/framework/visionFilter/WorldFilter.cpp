// Created by kjhertenberg on 13-5-19.
//
//

#include "WorldFilter.h"
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/Camera.h>


void WorldFilter::updateGeometry(const proto::SSL_GeometryData &geometry) {
    geometryData = GeometryData(geometry);
}

proto::World WorldFilter::getWorld(const Time &time) const {
    //TODO: split up in functions for robot and ball
    proto::World world;
    for(const auto& oneIDFilters : blue){
        if(!oneIDFilters.second.empty()){
            double maxHealth = - std::numeric_limits<double>::infinity();
            auto bestFilter = oneIDFilters.second.begin();
            for(auto robotFilter = oneIDFilters.second.begin(); robotFilter != oneIDFilters.second.end(); ++robotFilter){
                double health = robotFilter->getHealth();
                if(health > maxHealth){
                    maxHealth = health;
                    bestFilter = robotFilter;
                }
            }
            FilteredRobot bestRobot = bestFilter->mergeRobots(time);
            world.mutable_blue()->Add()->CopyFrom(bestRobot.asWorldRobot());
        }
    }
    for(const auto& oneIDFilters : yellow){
        if(!oneIDFilters.second.empty()){
            double maxHealth = - std::numeric_limits<double>::infinity();
            auto bestFilter = oneIDFilters.second.begin();
            for(auto robotFilter = oneIDFilters.second.begin(); robotFilter != oneIDFilters.second.end(); ++robotFilter){
                double health = robotFilter->getHealth();
                if(health > maxHealth){
                    maxHealth = health;
                    bestFilter = robotFilter;
                }
            }
            FilteredRobot bestRobot = bestFilter->mergeRobots(time);
            world.mutable_yellow()->Add()->CopyFrom(bestRobot.asWorldRobot());
        }
    }
    if(!balls.empty()){
        double maxHealth = - std::numeric_limits<double>::infinity();
        auto bestFilter = balls.begin();
        for (auto ballFilter = balls.begin(); ballFilter != balls.end(); ++ballFilter){
            double health = ballFilter->getHealth();
            if(health > maxHealth){
                maxHealth = health;
                bestFilter = ballFilter;
            }
        }
        FilteredBall bestBall = bestFilter->mergeBalls(time);
        world.mutable_ball()->CopyFrom(bestBall.asWorldBall());
    }
    world.set_time(time.asNanoSeconds());

    return world;
}

void WorldFilter::process(const std::vector<proto::SSL_DetectionFrame> &frames) {
    for(const auto& protoFrame : frames){
        DetectionFrame frame(protoFrame);
        //TODO: prune frame for AOI (area of interest) and balls detected within robots
        processRobots(frame, true);
        processRobots(frame, false);
        processBalls(frame);
    }
}


void WorldFilter::processRobots(const DetectionFrame &frame, bool blueBots) {
    robotMap& robots = blueBots ? blue : yellow;
    const std::vector<RobotObservation> & detectedRobots = blueBots ? frame.blue : frame.yellow;

    predictRobots(frame, robots);
    updateRobots(blueBots, robots, detectedRobots);
    updateRobotsNotSeen(frame, robots);
}

void WorldFilter::updateRobotsNotSeen(const DetectionFrame &frame, robotMap &robots){
    for(auto& oneIDFilters : robots){
        std::vector<RobotFilter>& filters = oneIDFilters.second;
        auto it = filters.begin();
        while(it!=filters.end()) {
            if(it->processNotSeen(frame.cameraID,frame.timeCaptured)){
                    it = filters.erase(it);
            } else{
                it++;
            }
        }
    }
}

void WorldFilter::updateRobots(bool blueBots, robotMap& robots,
                               const std::vector<RobotObservation> &detectedRobots){
    //add detected robots to existing filter(s) or create a new filter if no filter accepts the robot
    for (const auto& detectedRobot : detectedRobots){
        if(detectedRobot.robotId< 0 || detectedRobot.robotId >= 16){
            continue;
        }
        std::vector<RobotFilter>& oneIDFilters = robots[detectedRobot.robotId];
        bool accepted = false;
        for(RobotFilter& filter : oneIDFilters){
            accepted |= filter.processDetection(detectedRobot);
        }
        if(!accepted && oneIDFilters.size() < MAX_ROBOTFILTERS){
            oneIDFilters.emplace_back(RobotFilter(detectedRobot, blueBots));
        }
    }
}

void WorldFilter::predictRobots(const DetectionFrame &frame, robotMap &robots){
    for(auto& oneIDFilters : robots){
        for(auto& filter : oneIDFilters.second){
            filter.predictCam(frame.cameraID,frame.timeCaptured);
        }
    }
}

void WorldFilter::processBalls(const DetectionFrame &frame) {
    for(auto& filter : balls){
        filter.predictCam(frame.cameraID,frame.timeCaptured,geometryData);
    }
    for (const auto& detectedBall : frame.balls){
        bool accepted = false;
        for(BallFilter& ballFilter : balls){
            accepted |= ballFilter.processDetection(detectedBall);
        }
        if(!accepted && balls.size() < MAX_BALLFILTERS){
            balls.emplace_back(BallFilter(detectedBall));
        }
    }
    // process balls that weren't seen and remove them if necessary
    auto it = balls.begin();
    while(it!=balls.end()) {
        if(it->processNotSeen(frame.cameraID,frame.timeCaptured)){
            it = balls.erase(it);
        } else{
            it++;
        }
    }
}
