//
// Created by rolf on 30-09-20.
//

#include "BallAssigment.h"

BallAssignmentResult
assignBalls(const std::vector<BallPredictions> &predictions, const std::vector<BallObservation> &observations) {
    BallAssignmentResult assignment;
    for (const auto &observation : observations) {
        double closestDistance = std::numeric_limits<double>::infinity();
        CameraBallGroundEKF::PredictedBall ball;
        int objectID = -1;
        for (const auto &prediction : predictions) {
            for (const auto &predict : prediction.balls.balls) {
                double distance = (predict.position - observation.position).norm();
                if (distance < closestDistance) {
                    closestDistance = distance;
                    ball = predict;
                    objectID = prediction.objectID;
                }
            }
        }
        if (closestDistance > 0.5) { //m: the ball is not close to any existing ball
            assignment.unpairedBalls.push_back(observation);
        } else {
            //TODO: make constructor
            CameraBallGroundEKF::ObservationPredictionPair pair;
            pair.prediction = ball;
            pair.objectID = objectID;
            pair.observations = {observation};
            //Check if anotherh ball exists which was assigned to the exact same prediction
            auto it = std::find_if(assignment.observationPredictionPair.begin(),
                                   assignment.observationPredictionPair.end(),
                                   [pair](const CameraBallGroundEKF::ObservationPredictionPair &opPair) { return
                                   opPair.objectID == pair.objectID &&
                                   opPair.prediction.collisions.size() == pair.prediction.collisions.size();
                                   });
            if (it == assignment.observationPredictionPair.end()){
                assignment.observationPredictionPair.push_back(pair);
            }else{
                it->observations.push_back(observation);
            }
        }
    }
    return assignment;
}
