//
// Created by rolf on 22-03-20.
//

#include "BallObservation.h"

BallObservation::BallObservation(int cameraID, Time timeCaptured, Time timeSent,
        const proto::SSL_DetectionBall& detectionBall) :
        cameraID(cameraID),
        timeCaptured(timeCaptured),
        timeSent(timeSent),
        position(detectionBall.x()/1000.0,detectionBall.y()/1000.0), //Position by SSL-Vision is given in millimeters
        pixelPosition(detectionBall.pixel_x(),detectionBall.pixel_y()),
        confidence(detectionBall.confidence()),
        area(detectionBall.area()),
        height(detectionBall.z()/1000.0){
}

