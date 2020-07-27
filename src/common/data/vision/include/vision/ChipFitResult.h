//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPFITRESULT_H
#define SOCCER_CHIPFITRESULT_H

#include <Eigen/Dense>

struct ChipFitResult {
    Eigen::Vector3d kickPos;
    Eigen::Vector3d kickVel;
    std::optional<Eigen::VectorXd> residuals;
};


#endif //SOCCER_CHIPFITRESULT_H
