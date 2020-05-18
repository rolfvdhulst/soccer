//
// Created by rolf on 23-2-20.
//

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
RobotState::RobotState(const proto::WorldRobot &robot, const proto::RobotInfo &info) :
_id(robot.id()),
_position{robot.pos()},
_angle{robot.angle()},
_velocity{robot.vel()},
_angularVelocity{robot.w()},
_parameters{RobotParameters(info)},
_shape(RobotShape(_position,centerToFront(),radius(),_angle))
{
}
const RobotShape& RobotState::shape() const {
    return _shape;
}
double RobotState::radius() const {
    return _parameters.radius;
}
double RobotState::centerToFront() const {
    return _parameters.centerToFront;
}
const RobotParameters &RobotState::parameters() const {
    return _parameters;
}
RobotState::RobotState(const proto::WorldRobot &robot, RobotParameters parameters) :
        _id(robot.id()),
        _position{robot.pos()},
        _angle{robot.angle()},
        _velocity{robot.vel()},
        _angularVelocity{robot.w()},
        _parameters{parameters},
        _shape{RobotShape(_position,centerToFront(),radius(),_angle)}{
}
