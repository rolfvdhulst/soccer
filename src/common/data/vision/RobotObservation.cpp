//
// Created by rolf on 22-03-20.
//

#include "RobotObservation.h"

RobotObservation::RobotObservation(int cameraID, Time timeCaptured, Time timeSent, const proto::SSL_DetectionRobot& detectionRobot) :
        cameraID(cameraID),
        timeCaptured(timeCaptured),
        timeSent(timeSent),
        robotId(detectionRobot.robot_id()),
        position(detectionRobot.x()/1000.0,detectionRobot.y()/1000.0), //Position by SSL-Vision is given in millimeters
        pixelPosition(detectionRobot.pixel_x(),detectionRobot.pixel_y()),
        orientation(detectionRobot.orientation()),
        height(detectionRobot.height()/1000.0),
        confidence(detectionRobot.confidence()){
}