//
// Created by rolf on 23-2-20.
//

#include <protobuf/WorldRobot.pb.h>
#include "RobotState.h"

const RobotID &RobotState::id() const {
    return _id;
}
const Vector2 &RobotState::pos() const {
    return _position;
}
const Angle &RobotState::angle() const {
    return _angle;
}
const Vector2 &RobotState::vel() const {
    return _velocity;
}
const Rotation &RobotState::angularVel() const {
    return _angularVelocity;
}
RobotState::RobotState(const proto::WorldRobot &robot) :
_id(robot.id()),
_position{robot.pos()},
_angle{robot.angle()},
_velocity{robot.vel()},
_angularVelocity{robot.w()}
{
}
