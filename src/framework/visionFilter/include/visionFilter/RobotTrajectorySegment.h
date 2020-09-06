//
// Created by rolf on 31-08-20.
//

#ifndef SOCCER_ROBOTTRAJECTORYSEGMENT_H
#define SOCCER_ROBOTTRAJECTORYSEGMENT_H

#include <math/geometry/Vector2.h>
#include <math/geometry/Angle.h>
#include <math/geometry/Rotation.h>
#include <math/geometry/RobotShape.h>
#include <core/Time.h>
struct RobotTrajectorySegment {
    RobotShape startPos;//Contains initial angle and position
    Vector2 vel;
    double angVel;
    double dt;
    Time startTime;
    Time endTime;
    bool isBlue;
    int robotID;
};


#endif //SOCCER_ROBOTTRAJECTORYSEGMENT_H
