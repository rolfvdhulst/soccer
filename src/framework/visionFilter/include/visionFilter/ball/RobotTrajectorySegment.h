//
// Created by rolf on 31-08-20.
//

#ifndef SOCCER_ROBOTTRAJECTORYSEGMENT_H
#define SOCCER_ROBOTTRAJECTORYSEGMENT_H

#include <math/geometry/Vector2.h>
#include <math/geometry/Angle.h>
#include <math/geometry/Rotation.h>
#include <math/geometry/RobotShape.h>
class RobotTrajectorySegment {
    RobotShape shape;//Contains initial angle and position
    Vector2 vel;
    Vector2 acc;
    Rotation angularVel;
    Rotation angularAcc;
    double dt;
    Time startTime;
    Time endTime;
};


#endif //SOCCER_ROBOTTRAJECTORYSEGMENT_H
