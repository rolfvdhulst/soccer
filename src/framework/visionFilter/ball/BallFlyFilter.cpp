//
// Created by rolf on 27-05-20.
//

#include "ball/BallFlyFilter.h"
#include "ball/kick/estimation/chip/ChipEstimator6DRotated.h"
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
SingleBallFlight::SingleBallFlight(const FlightStartEvent &event) :
start_event{event}{ }

void BallFlyFilter::addObservation(const BallObservation &observation) {
  for(auto& flight : possible_flights){
    flight.addObservation(observation);
  }
}
void BallFlyFilter::startPossibleFlight(const FlightStartEvent &start) {
  SingleBallFlight possible_flight(start);
  possible_flights.push_back(possible_flight);
}
FlyingPredictions BallFlyFilter::getPredictions() const {
  return FlyingPredictions(); //TODO implement
}
