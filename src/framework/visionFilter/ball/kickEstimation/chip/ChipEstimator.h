//
// Created by rolf on 19-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR_H
#define SOCCER_CHIPESTIMATOR_H

#include <field/CameraMap.h>
#include <vision/BallObservation.h>
#include <vision/ChipFitResult.h>

class ChipEstimator {
public:
    virtual std::optional<ChipFitResult>
    getChipEstimate(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera, bool computeResiduals = false) = 0;

//    virtual std::optional<ChipFitResult> chipEstimation(bool computeResiduals = false) = 0;
//
//    virtual bool addDetection(const BallObservation &observation, const CameraMap &camera) = 0;
protected:
    virtual ChipFitResult
    postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false) = 0;

    virtual std::optional<Eigen::VectorXd> solve() = 0;

    virtual bool setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera) = 0;
    //TODO: check performance difference fixed matrices vs Dynamic matrices. Perhaps we want to change the interface if this matters a lot
    Eigen::MatrixXd AMatrix;
    Eigen::VectorXd BVector;


};


#endif //SOCCER_CHIPESTIMATOR_H
