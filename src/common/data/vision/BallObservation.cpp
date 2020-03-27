//
// Created by rolf on 22-03-20.
//

#include "BallObservation.h"

BallObservation::BallObservation(int cameraID, Time timeCaptured, Time timeSent,
        proto::SSL_DetectionBall detectionBall) :
        cameraID(cameraID),
        timeCaptured(timeCaptured),
        timeSent(timeSent),
        ball(std::move(detectionBall)){ }

