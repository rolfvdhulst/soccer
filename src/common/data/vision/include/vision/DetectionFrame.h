//
// Created by rolf on 29-05-20.
//

#ifndef SOCCER_DETECTIONFRAME_H
#define SOCCER_DETECTIONFRAME_H


#include <vector>
#include <core/Time.h>
#include "BallObservation.h"
#include "RobotObservation.h"

struct DetectionFrame {
    explicit DetectionFrame(const proto::SSL_DetectionFrame &protoFrame);
    int cameraID;
    Time timeCaptured;
    Time timeSent;
    double dt = 0.0;
    std::vector<BallObservation> balls;
    std::vector<RobotObservation> blue;
    std::vector<RobotObservation> yellow;
};


#endif //SOCCER_DETECTIONFRAME_H
