//
// Created by rolf on 17-5-20.
//

#ifndef SOCCER_ROBOTPARAMETERS_H
#define SOCCER_ROBOTPARAMETERS_H

#include <protobuf/RobotInfo.h>

#include <math.h>

struct RobotParameters {
  explicit RobotParameters(const proto::RobotInfo &params) :
  radius{params.radius()},
  centerToFront{params.centertofrontdist()},
  height{params.height()},
  frontWidth{params.frontwidth()},
  dribblerWidth{params.dribblerwidth()}
  {};
  RobotParameters() :
  radius{0.09},
  centerToFront{cos(0.785)*0.09},
  height{0.15},
  frontWidth{sin(0.785)*0.09*2},
  dribblerWidth{sin(0.785)*0.09*2}{}
  double radius;
  double centerToFront;
  double height;
  double frontWidth;
  double dribblerWidth;

};

#endif //SOCCER_ROBOTPARAMETERS_H
