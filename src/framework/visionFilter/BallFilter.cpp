//
// Created by rolf on 05-08-20.
//

#include "BallFilter.h"

BallFilter::BallFilter(const BallObservation &observation)
        : ObjectFilter(),
          cameraFilters{std::make_pair(observation.cameraID, CameraBallGroundEKF(observation))} {
    acceptedBalls.push_back(observation);
}

BallPredictions BallFilter::predictCam(int cameraID, const Time &untilTime, const GeometryData &geometryData,
                                       const std::vector<RobotTrajectorySegment> &robotTrajectories) const {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter != cameraFilters.end()) {
        auto predictions = cameraFilter->second.predict(untilTime, geometryData, robotTrajectories);
        BallPredictions prediction;
        prediction.objectID = getObjectID();
        prediction.hadRequestedCamera = true;
        prediction.balls = predictions;
        return prediction;
    } else {
        BallPredictions prediction;
        prediction.objectID = getObjectID();
        prediction.hadRequestedCamera = false;
        //TODO: Loop over all camera's, merge predictions into one and pass this back to check for new balls
        //Ugly temporary hack:
        for(const auto& camera : cameraFilters){
            prediction.balls =camera.second.predict(untilTime,geometryData,robotTrajectories);
        }
        return prediction;
    }
}


double BallFilter::getHealth() const {
    double maxHealth = 0.0;
    for (const auto &filter : cameraFilters) {
        maxHealth = fmax(filter.second.getHealth(), maxHealth);
    }
    return maxHealth;
}

FilteredBall BallFilter::mergeBalls(const Time &time) const {
    double mergeFactor = 1.5;
    Eigen::Vector2d vel(0, 0);
    Eigen::Vector2d pos(0, 0);
    double totalPosUncertainty = 0;
    double totalVelUncertainty = 0;
    bool isVisible = false;
    for (const auto &filter : cameraFilters) {
        FilteredBall ball = filter.second.getEstimate(time, true);
        //Use the filter health and uncertainties for a weighted average of observations
        double weight = 100.0 / ball.health;
        double posWeight = pow(ball.posUncertainty * weight, -mergeFactor);
        double velWeight = pow(ball.velocityUncertainty * weight, -mergeFactor);
        pos += ball.pos * posWeight;
        vel += ball.vel * velWeight;
        totalPosUncertainty += posWeight;
        totalVelUncertainty += velWeight;
        isVisible |= ball.isVisible;
    }
    pos /= totalPosUncertainty;
    vel /= totalVelUncertainty;
    FilteredBall result;
    result.pos = pos;
    result.vel = vel;
    result.isVisible = isVisible;
    return result;
}
//bool BallFilter::acceptDetection(const BallObservation &observation) {
//    auto cameraFilter = cameraFilters.find(observation.cameraID);
//    if (cameraFilter != cameraFilters.end()){
//        bool accepted = cameraFilter->second.addObservation(observation);
//        acceptedBalls.push_back(observation);
//        return accepted;
//    }
//    //Check if it makes sense that we see the relative to the other camera's
//    bool accepted = true;
//    for (const auto &filter : cameraFilters) {
//        accepted &= filter.second.acceptObservation(observation);
//    }
//    if (accepted) {
//        if(!cameraFilters.empty()){
//            //We can initialize this new filter with information from the other filters, by giving it the initial speed the others detected
//            //TODO: make this a weighted average (using e.g. filter age / health?)
//            Eigen::Vector2d velocity{0, 0};
//            for (const auto &filter : cameraFilters) {
//                velocity += filter.second.getVelocity(observation.timeCaptured);
//            }
//            velocity /= cameraFilters.size();
//            cameraFilters.insert(std::make_pair(observation.cameraID, CameraBallGroundEKF(observation, velocity)));
//        } else{
//            cameraFilters.insert(std::make_pair(observation.cameraID,CameraBallGroundEKF(observation)));
//        }
//        acceptedBalls.push_back(observation);
//    }
//    return accepted;
//
//}

BallObservation BallFilter::lastDetection() const {
    return acceptedBalls.back();
}

bool BallFilter::processDetections(const CameraBallGroundEKF::ObservationPredictionPair &detections, int cameraID) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter == cameraFilters.end()) {
        //If we have detections assigned, then create a new camera filter for this camera
        if (detections.observation.has_value()){
            cameraFilters.insert(std::make_pair(cameraID,CameraBallGroundEKF(detections.observation.value(),detections.prediction.velocity)));
        }
        return false;
    }
    //Process all the accepted detections of the frame in the filter
    bool removeFilter = cameraFilter->second.processDetections(detections);
    if (removeFilter) {
        cameraFilters.erase(cameraFilter);
    }
    return cameraFilters.empty();
}