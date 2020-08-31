//
// Created by rolf on 29-08-20.
//

#ifndef SOCCER_BALLTRAJECTORYSEGMENT_H
#define SOCCER_BALLTRAJECTORYSEGMENT_H

#include <math/geometry/Vector2.h>
#include <core/Time.h>

/**
 * This class represents a segment of a ball trajectory where it's under constant deceleration
 *
 */
struct BallTrajectorySegment {
    Vector2 startPos;
    Vector2 startVel;
    Vector2 endPos;
    Vector2 acceleration;
    double acc;
    double dt;
    Time startTime;
    Time endTime;
};


#endif //SOCCER_BALLTRAJECTORYSEGMENT_H
