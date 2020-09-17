//
// Created by rolf on 01-06-20.
//

#ifndef SOCCER_FILTEREDROBOT_H
#define SOCCER_FILTEREDROBOT_H

#include <Eigen/Dense>
#include <protobuf/WorldRobot.pb.h>

struct FilteredRobot {
    [[nodiscard]] proto::WorldRobot asWorldRobot() const;
    int id;
    bool isBlue;
    Eigen::Vector2d pos;
    Eigen::Vector2d vel;
    double angle;
    double angularVel;
    double health;
    double posUncertainty;
    double velocityUncertainty;
    double angleUncertainty;
    double angularVelUncertainty;
};


#endif //SOCCER_FILTEREDROBOT_H
