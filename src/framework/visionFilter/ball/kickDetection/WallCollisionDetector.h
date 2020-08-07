//
// Created by rolf on 29-07-20.
//

#ifndef SOCCER_WALLCOLLISIONDETECTOR_H
#define SOCCER_WALLCOLLISIONDETECTOR_H

#include "CollisionEvent.h"
#include "VelocityChangeEvent.h"

class WallCollisionDetector {
public:
    std::optional<CollisionEvent> checkForCollision(const VelocityChangeEvent& velocityChange);
//private:
//    std::optional<CollisionEvent> checkSegment(const BallState& ballState,const Time& extrapolationTime)

};


#endif //SOCCER_WALLCOLLISIONDETECTOR_H
