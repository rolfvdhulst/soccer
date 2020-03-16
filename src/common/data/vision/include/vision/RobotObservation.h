//
// Created by rolf on 21-01-20.
//

#ifndef RTT_ROBOTOBSERVATION_H
#define RTT_ROBOTOBSERVATION_H
#include <core/Time.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
/**
 * A struct to keep robotData and time as one observation.
 */
struct RobotObservation {
    explicit RobotObservation(int cameraID, const Time &time, proto::SSL_DetectionRobot detectionRobot) : cameraID(cameraID), time(time), bot(std::move(detectionRobot)) {}
    int cameraID;
    Time time;
    proto::SSL_DetectionRobot bot;
};
#endif  // RTT_ROBOTOBSERVATION_H
