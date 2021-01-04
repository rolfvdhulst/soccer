//
// Created by rolf on 21-01-20.
//

#ifndef RTT_ROBOTOBSERVATION_H
#define RTT_ROBOTOBSERVATION_H
#include <core/Time.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <Eigen/Dense>
/**
 * A struct to keep robotData and time as one observation.
 */
struct RobotObservation {
    explicit RobotObservation(int cameraID, Time timeCaptured, Time timeSent, const proto::SSL_DetectionRobot& detectionRobot);
    int cameraID;
    Time timeCaptured;
    Time timeSent;
    int robotId;
    Eigen::Vector2d position;
    Eigen::Vector2d pixelPosition;
    double orientation;
    double confidence;
    double height;
};
#endif  // RTT_ROBOTOBSERVATION_H
