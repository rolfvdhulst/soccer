//
// Created by rolf on 21-01-20.
//

#ifndef RTT_BALLOBSERVATION_H
#define RTT_BALLOBSERVATION_H

#include <core/Time.h>
#include <Eigen/Dense>
#include <protobuf/messages_robocup_ssl_detection.pb.h>

struct BallObservation {
    BallObservation() = default;
    explicit BallObservation(int cameraID, Time timeCaptured, Time timeSent, const proto::SSL_DetectionBall& detectionBall);
    explicit BallObservation(int cameraID, Time timeCaptured, Time timeSent, Eigen::Vector2d position, Eigen::Vector2d pixelPosition, double confidence, unsigned int area, double height);
    int cameraID;
    Time timeCaptured;
    Time timeSent;
    Eigen::Vector2d position;
    Eigen::Vector2d pixelPosition;
    uint32_t area;
    double confidence; //practically useless?
    double height; //practically useless, is always the same
};
BallObservation mergeBallObservationsByArea(const std::vector<BallObservation>& observations);
#endif  // RTT_BALLOBSERVATION_H
