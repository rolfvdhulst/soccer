//
// Created by rolf on 17-5-20.
//

#ifndef SOCCER_ROBOTPARAMETERS_H
#define SOCCER_ROBOTPARAMETERS_H

#include <protobuf/RobotInfo.h>


struct RobotParameters {
  explicit RobotParameters(const proto::RobotInfo &params) :
  radius{params.radius()},
  centerToFront{params.centertofrontdist()},
  height{params.height()},
  frontWidth{params.frontwidth()},
  dribblerWidth{params.dribblerwidth()}
  {};
  double radius;
  double centerToFront;
  double height;
  double frontWidth;
  double dribblerWidth;

};

#endif //SOCCER_ROBOTPARAMETERS_H
