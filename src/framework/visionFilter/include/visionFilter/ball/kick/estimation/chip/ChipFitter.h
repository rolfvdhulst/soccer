//
// Created by rolf on 05-01-21.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_ESTIMATION_CHIP_CHIPFITTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_ESTIMATION_CHIP_CHIPFITTER_H_

#include "ChipEstimator6DRotated.h"
class ChipFitter {
 public:
  void addObservation(const BallObservation& observation, const Camera& camera);
  std::optional<ChipFitResult> getPrediction(bool computeResiduals);
  [[nodiscard]] std::optional<ChipFitResult> getCachedPrediction() const;
 private:
  ChipEstimator6DRotated multiple_cam_estimator;
  std::optional<ChipFitResult> last_fit;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_ESTIMATION_CHIP_CHIPFITTER_H_
