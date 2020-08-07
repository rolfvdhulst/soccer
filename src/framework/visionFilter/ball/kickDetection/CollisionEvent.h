//
// Created by rolf on 29-07-20.
//

#ifndef SOCCER_COLLISIONEVENT_H
#define SOCCER_COLLISIONEVENT_H

#include <core/Time.h>
#include <Eigen/Dense>

class CollisionEvent {
    enum Type{
        ROBOT_FRONT,
        ROBOT_HULL,
        WALL
    };
    Type type;
    Time timeEstimate;
    Eigen::Vector2d locationEstimate;
    Eigen::Vector2d reboundVelocityEstimate;
};


#endif //SOCCER_COLLISIONEVENT_H
