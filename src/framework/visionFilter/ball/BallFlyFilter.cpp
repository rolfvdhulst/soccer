//
// Created by rolf on 27-05-20.
//

#include "ball/BallFlyFilter.h"


void BallFlyFilter::addObservations(const std::vector<KickOPPair>& op_pairs) {
  for(const auto& pair: op_pairs){
    for(const auto& observation : pair.observations){
      possible_flights[pair.id].addObservation(observation);
    }
  }
}
void BallFlyFilter::startPossibleFlight(const KickEvent &start) {
  if(possible_flights.size() < MAX_NUM_FLIGHTS()) {
    KickTrajectory possible_flight(start);
    possible_flights.push_back(possible_flight);
  }
}
KickPredictions BallFlyFilter::getPredictions(Time time, const CameraMap& camera_map, int camera_id) const {
  KickPredictions predictions;
  for (int i = 0; i < possible_flights.size(); ++i) {
    predictions.predictions.push_back(possible_flights[i].getPrediction(time,camera_map,camera_id));
    predictions.ids.push_back(i);
  }
  return predictions;
}
