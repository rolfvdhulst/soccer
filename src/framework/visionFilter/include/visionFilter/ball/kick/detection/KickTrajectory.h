//
// Created by rolf on 25-12-20.
//

#ifndef SOCCER_KICKTRAJECTORY_H
#define SOCCER_KICKTRAJECTORY_H

#include <vision/BallObservation.h>
#include "KickEvent.h"
#include "ball/kick/estimation/chip/ChipFitter.h"
#include "ball/kick/estimation/ground/GroundFitter.h"
#include <field/CameraMap.h>
#include "ball/kick/estimation/chip/ChipFitResult.h"


struct GroundPrediction{
    Eigen::Vector2d position;
    Eigen::Vector2d velocity;
};

struct ChipPrediction{
    ChipState chip_prediction;
    Eigen::Vector2d predicted_projection;
};

struct KickPrediction{
  GroundPrediction ground_prediction;
  std::optional<ChipPrediction> chip_prediction;

  bool isCloseGround(const BallObservation& observation) const;
  bool isCloseChip(const BallObservation& observation) const;
};

enum TrajectoryType{
  UNDECIDED = 0,
  GROUND = 1,
  FLYING = 2
};
class KickTrajectory {
public:
    explicit KickTrajectory(KickEvent event);
    void addObservation(const BallObservation& observation);
    [[nodiscard]] KickPrediction getPrediction(Time time, const CameraMap& camera_map, int camera_id) const;
    [[nodiscard]] unsigned long maxSingleCamObservations() const;
    [[nodiscard]] std::vector<BallObservation> allObservations() const;
    [[nodiscard]] unsigned long numberOfObservations() const;
private:
    [[nodiscard]] std::optional<ChipPrediction> getFlyingPrediction(Time time, const CameraMap& camera_map, int camera_id) const;
    KickEvent kick_event;
    std::map<int,std::vector<BallObservation>> camera_observations;
    ChipFitter chip_estimator;
    GroundFitter ground_estimator;
    TrajectoryType type = UNDECIDED;
};


#endif //SOCCER_KICKTRAJECTORY_H
