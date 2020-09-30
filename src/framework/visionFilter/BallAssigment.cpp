//
// Created by rolf on 30-09-20.
//

#include "BallAssigment.h"

BallAssignmentResult assignBalls(const std::vector<BallPredictions>& predictions,const std::vector<BallObservation>& observations){
    BallAssignmentResult assignment;
    for (const auto& observation : observations){

        assignment.unpairedBalls.push_back(observation);
    }
    return assignment;
}
