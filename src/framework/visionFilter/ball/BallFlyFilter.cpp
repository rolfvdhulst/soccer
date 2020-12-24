//
// Created by rolf on 27-05-20.
//

#include "ball/BallFlyFilter.h"
#include <utility>
#include "ball/kick/estimation/chip/ChipEstimator6DRotated.h"
#include "Eigen/Dense"

void SingleBallFlight::addObservation(const BallObservation& observation) {
  camObservations[observation.cameraID].push_back(observation);
}
std::optional<ChipFitResult> SingleBallFlight::getFit(const CameraMap &cam_map) {
  if(camObservations.size() >= 3){
    ChipEstimator6DRotated estimator;
    return estimator.getChipEstimate(allObservations(),cam_map,false);
  }
  return std::nullopt;
}
std::vector<BallObservation> SingleBallFlight::allObservations() const {
  std::vector<BallObservation> all;
  for(const auto& singleCamObs : camObservations){
    for(const auto& obs : singleCamObs.second){
      all.push_back(obs);
    }
  }
  return all;
}
unsigned long SingleBallFlight::maxSingleCamObservations() const {
  unsigned long count = 0;
  for(const auto& singleCamObs : camObservations){
    count = std::max(count,singleCamObs.second.size());
  }
  return count;
}
SingleBallFlight::SingleBallFlight(FlightStartEvent event) :
start_event{std::move(event)}{ }

FlyingBallPrediction SingleBallFlight::prediction(Time time, const CameraMap &camera_map,int camera_id) const {
  if(fit.has_value()){
    FlyingBallPrediction ball_prediction;
    auto predicted_world_pos = fit->predictPosition(time);
    ball_prediction.world_position = predicted_world_pos;
    auto camera = camera_map.getCamera(camera_id);
    if(camera){
      ball_prediction.project_ground_pos = camera->linearProjectToHorizontalPlane(predicted_world_pos,0.0);
      //alternatively, we project and reproject using distortion; unclear which method is more accurate.
    }else{
      ball_prediction.project_ground_pos = Eigen::Vector2d(predicted_world_pos.x(),predicted_world_pos.y());
    }
    ball_prediction.time = time;
    ball_prediction.id = -1;//TODO: what is this?
    return ball_prediction;
  }else{
    //If we have camera frames from this camera, use the last frame from this camera
    //If not, use the last position from another camera
    //If no frames (just a starting event) use the position of the flight start event
    return FlyingBallPrediction(); //TODO: implement based on simple linear extrapolations/ just use the position we have last seen the ball at
  }
}
unsigned long SingleBallFlight::numberOfObservations() const {
  unsigned long total = 0;
  for(const auto& singleCamObs : camObservations){
    total+=singleCamObs.second.size();
  }
  return total;
}

void BallFlyFilter::addObservation(const BallObservation &observation) {
  for(auto& flight : possible_flights){
    flight.addObservation(observation);
  }
}
void BallFlyFilter::startPossibleFlight(const FlightStartEvent &start) {
  SingleBallFlight possible_flight(start);
  possible_flights.push_back(possible_flight);
}
FlyingPredictions BallFlyFilter::getPredictions(Time time, const CameraMap& camera_map, int camera_id) const {
  FlyingPredictions predictions;
  for(const auto& flight : possible_flights){
    predictions.predictions.push_back(flight.prediction(time,camera_map,camera_id));
  }
  return predictions;
}
bool FlyingBallPrediction::isClose(const BallObservation &observation) const{
  //TODO: implement
  return false;
}
