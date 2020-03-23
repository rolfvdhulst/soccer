//
// Created by rolf on 22-03-20.
//

#include "RobotObservation.h"

RobotObservation::RobotObservation(int cameraID, Time timeCaptured, Time timeSent, proto::SSL_DetectionRobot detectionRobot) :
        cameraID(cameraID),
        timeCaptured(timeCaptured),
        timeSent(timeSent),
        bot(std::move(detectionRobot)) {

}