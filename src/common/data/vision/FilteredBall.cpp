//
// Created by rolf on 01-06-20.
//

#include "FilteredBall.h"

proto::WorldBall FilteredBall::asWorldBall() const {
    proto::WorldBall msg;
    msg.mutable_pos()->set_x(pos.x());
    msg.mutable_pos()->set_y(pos.y());
    msg.mutable_vel()->set_x(vel.x());
    msg.mutable_vel()->set_y(vel.y());
    msg.set_visible(isVisible);
    return msg;
}
