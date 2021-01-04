//
// Created by rolf on 25-12-20.
//

#ifndef SOCCER_KICKTRAJECTORY_H
#define SOCCER_KICKTRAJECTORY_H

#include <vision/BallObservation.h>
#include "KickEvent.h"
#include "ball/kick/estimation/chip/ChipTrajectory.h"
#include "ball/kick/estimation/ground/GroundTrajectory.h"
#include <field/CameraMap.h>


struct GroundPrediction{
    Eigen::Vector2d position;
    Eigen::Vector2d velocity;
};

struct ChipPrediction{
    ChipState chip_prediction;
    Eigen::Vector2d predicted_projection;
};


struct KickPrediction{
};
class KickTrajectory {
public:
    KickTrajectory(KickEvent event);
    void addObservation(const BallObservation& observation);
    KickPrediction getPrediction(Time time, const CameraMap& camera_map, int camera_id);
    [[nodiscard]] unsigned long maxSingleCamObservations() const;
    std::vector<BallObservation> allObservations() const;
    [[nodiscard]] unsigned long numberOfObservations() const;
private:
    KickEvent kick_event;
    std::map<int,std::vector<BallObservation>> camera_observations;
    ChipState last_chip_estimation;
    GroundTrajectory last_ground_estimation;
};


#endif //SOCCER_KICKTRAJECTORY_H
