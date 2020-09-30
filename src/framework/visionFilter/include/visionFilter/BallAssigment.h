//
// Created by rolf on 30-09-20.
//

#ifndef SOCCER_BALLASSIGMENT_H
#define SOCCER_BALLASSIGMENT_H

#include "BallFilter.h"

struct BallAssignmentResult{
    std::vector<CameraBallGroundEKF::ObservationPredictionPair> observationPredictionPair;
    std::vector<BallObservation> unpairedBalls;
};

BallAssignmentResult assignBalls(const std::vector<BallPredictions>& predictions,const std::vector<BallObservation>& observations);


#endif //SOCCER_BALLASSIGMENT_H
