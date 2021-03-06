//
// Created by rolf on 21-01-20.
//

#ifndef RTT_BALLOBSERVATION_H
#define RTT_BALLOBSERVATION_H
#include <core/Time.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>

struct BallObservation {
    explicit BallObservation(int cameraID, Time timeCaptured, Time timeSent, proto::SSL_DetectionBall detectionBall);
    int cameraID;
    Time timeCaptured;
    Time timeSent;
    proto::SSL_DetectionBall ball;
};
#endif  // RTT_BALLOBSERVATION_H
