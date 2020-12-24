//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPFITRESULT_H
#define SOCCER_CHIPFITRESULT_H

#include <Eigen/Dense>
#include <core/Time.h>

struct ChipFitResult {
    Eigen::Vector3d kickPos;
    Eigen::Vector3d kickVel;
    Time kickTime;
    Eigen::Vector3d predictPosition(Time futureTime) const;
    std::optional<Eigen::VectorXd> residuals;
};


#endif //SOCCER_CHIPFITRESULT_H
