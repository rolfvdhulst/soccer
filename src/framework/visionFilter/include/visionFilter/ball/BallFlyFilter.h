//
// Created by rolf on 27-05-20.
//

#ifndef SOCCER_BALLFLYFILTER_H
#define SOCCER_BALLFLYFILTER_H

#include <map>
#include <vector>
#include <vision/BallObservation.h>
#include <vision/ChipFitResult.h>
#include <field/CameraMap.h>

class FlyingBallPrediction{
  Eigen::Vector3d world_position;
  Eigen::Vector2d project_ground_pos;
  Time time;
  std::size_t id;
};
class FlyingObservationPredictionPair{
  FlyingBallPrediction prediction;
  BallObservation observation;
};
struct FlyingPredictions{
  std::vector<FlyingBallPrediction> predictions;
};
enum FlightStartEventType{
  BOUNCE = 0,
  ROBOT_FRONT = 1,
  ROBOT_FRONT_WITH_COMMAND = 2,

};
class FlightStartEvent{
  //types: robot front hull collision | bounce | (velocity change)
  Eigen::Vector3d initial_pos;
  std::optional<Eigen::Vector3d> initial_vel;
  Time time;
  FlightStartEventType type;
};

class SingleBallFlight{
 public:
  explicit SingleBallFlight(const FlightStartEvent& event);
  void addObservation(const BallObservation& observation);
  std::optional<ChipFitResult> getFit(const CameraMap& cam_map);
  [[nodiscard]] unsigned long maxSingleCamObservations() const;
  [[nodiscard]] std::vector<BallObservation> allObservations() const;
 private:
  FlightStartEvent start_event;
  std::map<int,std::vector<BallObservation>> camObservations;
};

class BallFlyFilter {
 public:
  [[nodiscard]] FlyingPredictions getPredictions() const;
  void startPossibleFlight(const FlightStartEvent& start);
  void addObservation(const BallObservation& observation);
 private:
  std::optional<SingleBallFlight> current_flight;
  std::vector<SingleBallFlight> possible_flights;
};


#endif //SOCCER_BALLFLYFILTER_H
