//
// Created by rolf on 19-07-20.
//

#include "ChipFitResult.h"
Eigen::Vector3d ChipFitResult::predictPosition(Time futureTime) const {
  double time = (futureTime-kickTime).asSeconds();//Time difference
  return kickPos + time * kickVel + 0.5 * time * time * Eigen::Vector3d(0,0,-9.81);
}