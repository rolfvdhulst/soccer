//
// Created by rolf on 31-08-20.
//

#include "RobotTrajectorySegment.h"

Vector2 RobotTrajectorySegment::endPos() const {
    return startPos.pos() + vel*dt;
}

Angle RobotTrajectorySegment::endAngle() const {
    return startPos.angle() + angVel*dt;
}
