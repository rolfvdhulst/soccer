//
// Created by rolf on 01-06-20.
//

#include "FilteredRobot.h"

proto::WorldRobot FilteredRobot::asWorldRobot() const {
    proto::WorldRobot msg;
    msg.set_id(id);
    msg.mutable_pos()->set_x(pos.x());
    msg.mutable_pos()->set_y(pos.y());
    msg.set_angle(angle);
    msg.mutable_vel()->set_x(vel.x());
    msg.mutable_vel()->set_y(vel.y());
    msg.set_w(angularVel);
    return msg;
}
