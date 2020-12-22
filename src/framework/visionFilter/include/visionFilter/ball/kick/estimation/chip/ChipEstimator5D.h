//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR5D_H
#define SOCCER_CHIPESTIMATOR5D_H

#include "ChipEstimator.h"

class ChipEstimator5D {
public:
    std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false);

protected:
    Eigen::MatrixXd AMatrix;
    Eigen::VectorXd BVector;
    struct ChipFitResultInternal{
        Eigen::VectorXd solution;
        Eigen::VectorXd residuals;
        [[nodiscard]] double l1Error() const;
        [[nodiscard]] double l2Error() const;
    };

    ChipFitResult postProcess(const ChipFitResultInternal &solution);

    std::optional<ChipFitResult> timeLineSearch(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera);

    std::optional<ChipFitResultInternal> computeL2Sol(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, double tOffSet);
    std::optional<ChipFitResultInternal> computeL1Sol(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, double tOffSet);


    bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, double tOffSet);
};


#endif //SOCCER_CHIPESTIMATOR5D_H
