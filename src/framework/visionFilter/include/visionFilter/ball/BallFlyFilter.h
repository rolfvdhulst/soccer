//
// Created by rolf on 27-05-20.
//

#ifndef SOCCER_BALLFLYFILTER_H
#define SOCCER_BALLFLYFILTER_H

#include "ball/kick/detection/KickTrajectory.h"
#include "ball/kick/detection/KickEvent.h"

struct KickPredictions{
  std::vector<KickPrediction> predictions;
  std::vector<unsigned long> ids;
};

struct KickOPPair{
  KickPrediction prediction;
  int id;
  std::vector<BallObservation> observations;
};
class BallFlyFilter {
 public:
  [[nodiscard]] KickPredictions getPredictions(Time time,const CameraMap& camera_map, int camera_id) const;
  void startPossibleFlight(const KickEvent& start);
  void addObservations(const std::vector<KickOPPair>& op_pairs);
  void update();
 private:
  static constexpr int MAX_NUM_FLIGHTS() {
    return 2;
  };
  std::optional<KickTrajectory> current_flight;
  std::vector<KickTrajectory> possible_flights;
};


#endif //SOCCER_BALLFLYFILTER_H
