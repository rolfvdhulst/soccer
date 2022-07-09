//
// Created by rolf on 30-09-20.
//

#ifndef SOCCER_BALLASSIGMENT_H
#define SOCCER_BALLASSIGMENT_H

#include "BallFilter.h"

struct BallAssignmentResult{
    std::map<int, BallGroundFilter::ObservationPredictionPair> observationPredictionPairs;
    std::vector<BallGroundFilter::ObservationPredictionPair> splitPairs;
    std::vector<BallObservation> unpairedObservations;

    std::map<int,KickOPPair> kicked_balls;
};

struct preOPPair{
    BallGroundFilter::PredictedBall prediction;
    std::vector<BallObservation> observations;
    int objectID;
    [[nodiscard]] double closestObservationDistance() const;
    [[nodiscard]] std::pair<std::optional<BallObservation>,std::vector<BallObservation>> splitObservations() const;
};

std::map<int,std::vector<preOPPair>> createInitialAssignment(const std::map<int,BallPredictions> & predictions);
std::vector<BallObservation> mergeVeryCloseBalls(const std::vector<BallObservation>& observations);
BallAssignmentResult assignBalls(const std::map<int,BallPredictions> &predictions,const std::vector<BallObservation>& observations);

#endif //SOCCER_BALLASSIGMENT_H
