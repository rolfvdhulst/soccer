//
// Created by rolf on 11-08-20.
//

#ifndef SOCCER_BALLCOLLISIONRESULT_H
#define SOCCER_BALLCOLLISIONRESULT_H

#include <Eigen/Dense>
#include <core/Time.h>
/**
 * This struct contains an (estimated) velocity outwards from a collision
 */

struct BallCollisionResult {
    Eigen::Vector2d collisionPos;
    Time collisionTime = Time(-1.0); //might not be set by some checkers
    Eigen::Vector2d outVelocity = Eigen::Vector2d::Zero();
};


#endif //SOCCER_BALLCOLLISIONRESULT_H
