//
// Created by rolf on 21-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR6DROTATED_H
#define SOCCER_CHIPESTIMATOR6DROTATED_H

#include "ChipEstimator.h"

class ChipEstimator6DRotated : public ChipEstimator{
public:
    std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false) override;

protected:
    ChipFitResult postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false) override;

    std::optional<Eigen::VectorXd> solve() override;

    std::optional<Eigen::VectorXd> solveL2norm();
    std::optional<Eigen::VectorXd> solveIRLSL1norm();

    bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera) override;
};


#endif //SOCCER_CHIPESTIMATOR6DROTATED_H
