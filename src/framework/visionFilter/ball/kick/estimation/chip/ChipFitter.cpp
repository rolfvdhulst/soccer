//
// Created by rolf on 05-01-21.
//

#include "ball/kick/estimation/chip/ChipFitter.h"
void ChipFitter::addObservation(const BallObservation &observation, const Camera &camera) {
  if(multiple_cam_estimator.nrOfObservations() == 0){
    multiple_cam_estimator = ChipEstimator6DRotated(observation,camera);
  }else{
    multiple_cam_estimator.addObservation(observation,camera);
  }

}
std::optional<ChipFitResult> ChipFitter::getPrediction(bool computeResiduals) {
  auto estimation = multiple_cam_estimator.getChipEstimate(computeResiduals);
  last_fit = estimation; //cache for later use, the above computation is quite expensive.
  return estimation;
}
std::optional<ChipFitResult> ChipFitter::getCachedPrediction() const {
  return last_fit;
}
