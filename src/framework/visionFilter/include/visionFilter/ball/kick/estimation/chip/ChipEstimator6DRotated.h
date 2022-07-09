//
// Created by rolf on 21-07-20.
//

#ifndef SOCCER_CHIPESTIMATOR6DROTATED_H
#define SOCCER_CHIPESTIMATOR6DROTATED_H

#include "ChipEstimator.h"

class ChipEstimator6DRotated {
 public:
  ChipEstimator6DRotated() = default;
  ChipEstimator6DRotated(const BallObservation& observation, const Camera& camera);
  std::optional<ChipFitResult> getChipEstimate(bool computeResiduals = false);
  void addObservation(const BallObservation &observation, const Camera &camera);
  bool setObservations(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &cameras);
  unsigned long nrOfObservations() const;
 protected:
  ChipFitResult postProcess(const Eigen::VectorXd &solution, bool computeResiduals = false);
  std::optional<Eigen::VectorXd> solve();

  std::optional<Eigen::VectorXd> solveL2norm();
  std::optional<Eigen::VectorXd> solveIRLSL1norm();

  Time firstTime;
  Eigen::MatrixXd AMatrix;
  Eigen::VectorXd BVector;
};

#endif //SOCCER_CHIPESTIMATOR6DROTATED_H
