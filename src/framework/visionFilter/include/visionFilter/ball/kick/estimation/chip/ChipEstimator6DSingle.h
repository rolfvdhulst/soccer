//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR6DSINGLE_H
#define SOCCER_CHIPESTIMATOR6DSINGLE_H

#include "ChipEstimator.h"
class ChipEstimator6DSingle : public ChipEstimator {
public:
    std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false) override;

protected:
    ChipFitResult postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false) override;

    std::optional<Eigen::VectorXd> solve() override;

    bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera) override;

    Camera usedCamera;
};


#endif //SOCCER_CHIPESTIMATOR6DSINGLE_H
