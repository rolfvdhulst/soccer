//
// Created by rolf on 25-12-20.
//

#include "ball/kick/detection/KickTrajectory.h"
#include <utility>
KickTrajectory::KickTrajectory(KickEvent event) :
kick_event(std::move(event)){

}
void KickTrajectory::addObservation(const BallObservation &observation) {
  camera_observations[observation.cameraID].push_back(observation);
}
unsigned long KickTrajectory::maxSingleCamObservations() const {
  unsigned long count = 0;
  for(const auto& singleCamObs : camera_observations){
    count = std::max(count,singleCamObs.second.size());
  }
  return count;
}
unsigned long KickTrajectory::numberOfObservations() const {
  unsigned long total = 0;
  for(const auto& singleCamObs : camera_observations){
    total+=singleCamObs.second.size();
  }
  return total;
}
std::vector<BallObservation> KickTrajectory::allObservations() const {
  std::vector<BallObservation> all;
  for(const auto& singleCamObs : camera_observations){
    for(const auto& obs : singleCamObs.second){
      all.push_back(obs);
    }
  }
  return all;
}
KickPrediction KickTrajectory::getPrediction(Time time, const CameraMap &camera_map, int camera_id) const {
}
std::optional<ChipPrediction> KickTrajectory::getFlyingPrediction(Time time, const CameraMap &camera_map, int camera_id) const {
  auto cached_prediction = chip_estimator.getCachedPrediction();

  if(cached_prediction.has_value()){
    auto predicted_state = cached_prediction->state.prediction(time);
    Eigen::Vector3d predicted_world_pos = predicted_state.pos();
    auto camera = camera_map.getCamera(camera_id);
    Eigen::Vector2d project_ground_pos;
    if(camera){
      project_ground_pos = camera->linearProjectToHorizontalPlane(predicted_world_pos,0.0);
      //alternatively, we project and reproject using distortion; unclear which method is more accurate.
    }else{
      project_ground_pos = Eigen::Vector2d(predicted_world_pos.x(),predicted_world_pos.y());
    }
    return ChipPrediction{
      .chip_prediction = predicted_state,
      .predicted_projection = project_ground_pos
    };
  }else{
    //In this case, we use the same method as for the ground predictions, so we make this null
    return std::nullopt;
  }

}
bool KickPrediction::isCloseGround(const BallObservation &observation) const {
  return (observation.position - ground_prediction.position).norm() < 0.1; //TODO: tune
}
bool KickPrediction::isCloseChip(const BallObservation &observation) const {
  if(chip_prediction.has_value()){
    return (observation.position - chip_prediction->predicted_projection).norm() < 0.2; //TODO: tune
  }
  return false;
}
