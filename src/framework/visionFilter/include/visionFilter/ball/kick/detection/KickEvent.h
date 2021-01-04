//
// Created by rolf on 25-12-20.
//

#ifndef SOCCER_KICKEVENT_H
#define SOCCER_KICKEVENT_H

#include <Eigen/Dense>
#include <core/Time.h>

enum KickEventType{
    UNKNOWN = 0,
    ROBOT_FRONT_WITHOUT_COMMAND = 1,
    ROBOT_FRONT_WITH_COMMAND = 2,
    ROBOT_FRONT_UNKNOWN_COMMAND = 3
};

class KickEvent {
    KickEventType type;
    Eigen::Vector3d initial_pos;
    std::optional<Eigen::Vector3d> initial_vel;
    Time time;
};



#endif //SOCCER_KICKEVENT_H
