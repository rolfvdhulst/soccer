//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPFITRESULT_H
#define SOCCER_CHIPFITRESULT_H

#include <Eigen/Dense>
#include <core/Time.h>
#include "ChipTrajectory.h"

struct ChipFitResult {
  ChipFitResult(Eigen::Vector3d position, Eigen::Vector3d velocity, Time time);
    ChipState state;
    std::optional<Eigen::VectorXd> residuals;
};


#endif //SOCCER_CHIPFITRESULT_H
