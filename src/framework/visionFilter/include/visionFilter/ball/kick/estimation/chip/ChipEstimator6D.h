//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR6D_H
#define SOCCER_CHIPESTIMATOR6D_H


#include "ChipEstimator.h"

class ChipEstimator6D {
public:
    std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false);

protected:
    ChipFitResult postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false);

    std::optional<Eigen::VectorXd> solve();

    std::optional<Eigen::VectorXd> solveL2norm();
    std::optional<Eigen::VectorXd> solveIRLSL1norm();

    bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera);
};


#endif //SOCCER_CHIPESTIMATOR6D_H
