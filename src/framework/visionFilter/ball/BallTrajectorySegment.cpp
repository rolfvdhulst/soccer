//
// Created by rolf on 29-08-20.
//

#include "ball/BallTrajectorySegment.h"

LineSegment BallTrajectorySegment::path() const{
    return LineSegment(startPos,endPos);
}

Vector2 BallTrajectorySegment::getVelocity(double time) const {
    return startVel+startVel.normalize()*acc*time;
}

Vector2 BallTrajectorySegment::getPosition(double time) const {
    return startPos + startVel*time + startVel.normalize()*acc*time*time*0.5;
}
