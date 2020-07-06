//
// Created by rolf on 01-06-20.
//

#ifndef SOCCER_FILTEREDBALL_H
#define SOCCER_FILTEREDBALL_H

#include <Eigen/Dense>
#include <protobuf/WorldBall.pb.h>

struct FilteredBall {
    [[nodiscard]] proto::WorldBall asWorldBall() const;
    Eigen::Vector2d pos;
    Eigen::Vector2d vel;
    double health;
    double posUncertainty;
    double velocityUncertainty;
    bool isVisible;
};


#endif //SOCCER_FILTEREDBALL_H
