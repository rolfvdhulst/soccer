//
// Created by rolf on 30-09-20.
//

#include "BallAssigment.h"

BallAssignmentResult
assignBalls(const std::map<int, BallPredictions> &predictions, const std::vector<BallObservation> &observations) {
    BallAssignmentResult assignment;
    auto assignmentMap = createInitialAssignment(predictions);
    //First pass: Simply assign balls to closest target
    for (const auto &observation : observations) {
        CameraBallGroundEKF::PredictedBall ball;
        double closestDistance = std::numeric_limits<double>::infinity();
        int objectID = -1;
        for (const auto &prediction : predictions) {
            for (const auto &predict : prediction.second.balls.balls) {
                double distance = (predict.position - observation.position).norm();
                if (distance < closestDistance) {
                    closestDistance = distance;
                    ball = predict;
                    objectID = prediction.first;
                }
            }
        }
        if (closestDistance > 0.3) { //m: the ball is not close to any existing ball
            assignment.unpairedObservations.push_back(observation);
        } else {
            auto it = std::find_if(assignmentMap.at(objectID).begin(),
                                   assignmentMap.at(objectID).end(),
                                   [ball](const preOPPair &opPair) {
                                       return
                                               ball.collisions.size() == opPair.prediction.collisions.size();
                                   });
            if (it != assignmentMap.at(objectID).end()) {
                it->observations.push_back(observation);
            } else {
                std::cerr << "Could not match observation!!" << std::endl;
                throw std::invalid_argument("invalid argument");
            }
        }
    }
    //If more than one ball was assigned redivide so that each object can only get one ball
    for(const auto& singleObjectPredictions : assignmentMap){
        int numberOfSeenPredictions = 0;
        for(const auto& prediction : singleObjectPredictions.second){
            if(!prediction.observations.empty()){
                ++numberOfSeenPredictions;
            }
        }
        if(numberOfSeenPredictions == 0){
            //Choose prediction with most collisions TODO: should probably check if this actually one with most collisions
            const auto& bestPrediction = singleObjectPredictions.second.back();
            CameraBallGroundEKF::ObservationPredictionPair opPair;
            opPair.prediction = bestPrediction.prediction;
            opPair.objectID = bestPrediction.objectID;

            assignment.observationPredictionPairs[singleObjectPredictions.first] = opPair;
        } else{
            //Choose the one for which the ball is closest as the direct continuation.
            if(singleObjectPredictions.second.size()>1){
                double point = 0.1;
            }
            const auto bestPrediction = std::min_element(singleObjectPredictions.second.begin(),singleObjectPredictions.second.end(),
                                                         [](const preOPPair& a, const preOPPair&b){
                return a.closestObservationDistance() < b.closestObservationDistance();
            });

            //For the others, make new filters if they have one or more observations
            //For each if it has more than 2 individual observations, pick the closest, use that, and merge any balls that are very close to it with it
            const auto [observed,discarded] = bestPrediction->splitObservations();
            CameraBallGroundEKF::ObservationPredictionPair opPair;
            opPair.prediction = bestPrediction->prediction;
            opPair.objectID = bestPrediction->objectID;
            opPair.observation = observed;
            assignment.observationPredictionPairs[singleObjectPredictions.first] = opPair;
            //Add discarded observations to the unpaired list
            for(const auto& observation : discarded){
                assignment.unpairedObservations.push_back(observation);
            }
            for(auto it = singleObjectPredictions.second.begin(); it!= singleObjectPredictions.second.end(); it++){
                if(it == bestPrediction){
                    continue;
                }
                const auto [observed,discarded] = it->splitObservations();
                if(observed.has_value()){
                    CameraBallGroundEKF::ObservationPredictionPair otherObjectOPPair;
                    otherObjectOPPair.prediction = it->prediction;
                    otherObjectOPPair.objectID = singleObjectPredictions.first;
                    otherObjectOPPair.observation = observed;
                    assignment.splitPairs.push_back(otherObjectOPPair);
                }
                for(const auto& observation : discarded){
                    assignment.unpairedObservations.push_back(observation);
                }
            }
        }

    }
    //For each object pick the best option.
    return assignment;
}

std::map<int, std::vector<preOPPair>> createInitialAssignment(const std::map<int, BallPredictions> &predictions) {
    std::map<int, std::vector<preOPPair>> initialMap;
    for (const auto &elem : predictions) {
        for (const auto &ball : elem.second.balls.balls) {
            preOPPair pair;
            pair.objectID = elem.second.objectID;
            pair.prediction = ball;
            initialMap[elem.first].emplace_back(pair);
        }
    }
    return initialMap;
}

double preOPPair::closestObservationDistance() const {
    double closestDistance = std::numeric_limits<double>::infinity();
    for(const auto& observation : observations){
        double distance = (prediction.position-observation.position).norm();
        if(distance < closestDistance){
            closestDistance = distance;
        }
    }
    return closestDistance;
}

std::pair<std::optional<BallObservation>, std::vector<BallObservation>> preOPPair::splitObservations() const{
    //The optional represents the best ball for this prediction (possibly merged), whereas the vector represents balls that were discarded as
    //not being 'good enough'
    std::pair<std::optional<BallObservation>,std::vector<BallObservation>> pair;
    if(observations.empty()){
        return pair;
    }
    //Find the closest to the prediction. TODO: check if this works
    if(observations.size()>1){
        double point = 0.0;
    }
    auto best = std::min_element(observations.begin(),observations.end(),[this](const BallObservation& a, const BallObservation &b){
        return (a.position-prediction.position).squaredNorm()<(b.position-prediction.position).squaredNorm();
    });
    std::vector<BallObservation> mergeableBalls;
    std::vector<BallObservation> discardedBalls;
    for(const auto&  observation : observations ){
        if((observation.position - best->position).norm() < 0.02){ //TODO: magic number
            mergeableBalls.push_back(observation);
        }else{
            discardedBalls.push_back(observation);
        }
    }
    //The best ball should always by definition be in the mergeable balls
    BallObservation bestBall = mergeableBalls.size() >1 ? mergeBallObservationsByArea(mergeableBalls) : mergeableBalls[0];
    pair.first = bestBall;
    pair.second = discardedBalls;
    return pair;
}
