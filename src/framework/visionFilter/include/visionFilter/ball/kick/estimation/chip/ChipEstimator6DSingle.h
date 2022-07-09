//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR6DSINGLE_H
#define SOCCER_CHIPESTIMATOR6DSINGLE_H

#include "ChipEstimator.h"
class ChipEstimator6DSingle{
public:
    std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false);

protected:
    ChipFitResult postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false);

    std::optional<Eigen::VectorXd> solve();

    bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera);

    Camera usedCamera;
};


#endif //SOCCER_CHIPESTIMATOR6DSINGLE_H
