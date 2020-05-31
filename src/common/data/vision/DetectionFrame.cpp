//
// Created by rolf on 29-05-20.
//

#include "DetectionFrame.h"

DetectionFrame::DetectionFrame(const proto::SSL_DetectionFrame &protoFrame) :
cameraID{},
timeCaptured{},
timeSent{}
{
    for(const auto& ball : protoFrame.balls()){
        balls.emplace_back(BallObservation(cameraID,timeCaptured,timeSent,ball));
    }
    for(const auto& blueBot : protoFrame.robots_blue()){
        blue.emplace_back(RobotObservation(cameraID,timeCaptured,timeSent,blueBot));
    }
    for(const auto& yellowBot : protoFrame.robots_yellow()){
        yellow.emplace_back(RobotObservation(cameraID,timeCaptured,timeSent,yellowBot));
    }
}
