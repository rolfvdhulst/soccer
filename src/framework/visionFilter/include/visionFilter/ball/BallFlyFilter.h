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


struct RollingBallPrediction{
    Eigen::Vector2d predicted_roll_position;
};
struct FlyingBallPrediction{
  Eigen::Vector3d world_position;
  Eigen::Vector2d project_ground_pos;

  [[nodiscard]] bool isClose(const BallObservation& observation) const;
};
struct KickedBallPrediction{
    RollingBallPrediction rolling_prediction;
    FlyingBallPrediction flying_prediction;
    Time time;
    std::size_t id;
};

struct FlyingObservationPrediction{
  FlyingBallPrediction prediction;
  std::vector<BallObservation> observations;
};
struct FlyingPredictions{
  std::vector<FlyingBallPrediction> predictions;
};


class SingleBallFlight{
 public:
  explicit SingleBallFlight(FlightStartEvent  event);
  void addObservation(const BallObservation& observation);
  [[nodiscard]] FlyingBallPrediction prediction(Time time,const CameraMap& cam_map,int camera_id) const;
  std::optional<ChipFitResult> getFit(const CameraMap& cam_map);

  [[nodiscard]] unsigned long maxSingleCamObservations() const;
  [[nodiscard]] std::vector<BallObservation> allObservations() const;
  [[nodiscard]] unsigned long numberOfObservations() const;
 private:
  FlightStartEvent start_event;
  std::map<int,std::vector<BallObservation>> camObservations;
  std::optional<ChipFitResult> fit;
};

class BallFlyFilter {
 public:
  [[nodiscard]] FlyingPredictions   getPredictions(Time time,const CameraMap& camera_map, int camera_id) const;
  void startPossibleFlight(const FlightStartEvent& start);
  void addObservation(const BallObservation& observation);
 private:
  std::optional<SingleBallFlight> current_flight;
  std::vector<SingleBallFlight> possible_flights;
};


#endif //SOCCER_BALLFLYFILTER_H
