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
            oneIDFilters.emplace_back(RobotFilter(robot));
        }
    }
}

void CameraFilter::processBalls(const DetectionFrame &frame) {
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
        if(!accepted){
            balls.emplace_back(BallFilter(ball));
        }
    }
}

void CameraFilter::updateCalibration(const Camera &calibration) {
    camera=calibration;
}

void CameraFilter::updateRobotInfo(const proto::TeamRobotInfo& robotInfo) {
    teamInfo = robotInfo;
}
