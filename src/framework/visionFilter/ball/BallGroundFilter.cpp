//
// Created by rolf on 10-08-20.
//

#include "ball/BallGroundFilter.h"

#include <utility>
#include <FilterConstants.h>
#include <visionMatlab/VisionMatlabLogger.h>
#include "ball/collision/CollisionChecker.h"

BallGroundFilter::BallGroundFilter(const BallObservation &observation, Eigen::Vector2d velocityEstimate) :
        CameraObjectFilter(0.2, 1 / 60.0, 15, 3, observation.timeCaptured) {
    Eigen::Vector4d startState = {observation.position.x(), observation.position.y(), velocityEstimate.x(),
                                  velocityEstimate.y()};

    Eigen::Matrix4d startCovariance = Eigen::Matrix4d::Zero();
    startCovariance(0, 0) = BALL_POSITION_INITIAL_COV;
    startCovariance(1, 1) = BALL_POSITION_INITIAL_COV;
    startCovariance(2, 2) = BALL_VELOCITY_INITIAL_COV;
    startCovariance(3, 3) = BALL_VELOCITY_INITIAL_COV;

    ekf = BallGroundEKF(startState, startCovariance, 1.0, BALL_POSITION_MEASUREMENT_ERROR, observation.timeCaptured);
//    registerLogFile(observation.position);

}

//void CameraBallGroundEKF::registerLogFile(const Eigen::Vector2d &observation) {
//    if (!matlab_logger::logger.isCurrentlyLogging()) {
//        return;
//    }
//    int id = matlab_logger::logger.writeNewFilter(4, 2, VisionMatlabLogger::BALL_FILTER);
//    setID(id);
//    writeLogFile(observation);
//}

//void CameraBallGroundEKF::writeLogFile(const Eigen::Vector2d &observation) {
//    if (!matlab_logger::logger.isCurrentlyLogging()) {
//        return;
//    }
//    int id = getID();
//    if (id > 0) {
//        matlab_logger::logger.writeData(
//                id,
//                ekf.lastUpdated().asSeconds(),
//                observation,
//                ekf.state(),
//                ekf.covariance(),
//                ekf.innovation()
//        );
//    } else {
//        registerLogFile(observation);
//    }
//
//}

void BallGroundFilter::update(const BallObservation &observation) {
    ekf.update(observation.position);
    objectSeen(observation.timeCaptured);
//    writeLogFile(observation.position);
}

bool BallGroundFilter::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3;
}

Eigen::Vector2d BallGroundFilter::getVelocity(const Time &time) const {
    return ekf.getVelocityEstimate(time);
}

BallGroundFilter::PredictedBalls BallGroundFilter::predict(Time time, const GeometryData &geometryData,
                                                           const std::vector<RobotTrajectorySegment> &robotTrajectories) const {
    //TODO: pack this in a function
    PredictedBalls predictedBalls;
    BallTrajectorySegment segment = ekf.getSegment(time);
    PredictedBall ball;
    ball.position = segment.endPos;
    ball.time = time;
    predictedBalls.balls.push_back(ball);
    std::vector<CollisionChecker::Collision> collisions;
    if (!(segment.startPos ==
          segment.endPos)) { //TODO: fix this ball lying still case (maybe just pass velocity 0, acc 0?)
        int maxCollisions = 3;
        int i = 0;
        while (i < maxCollisions) {
            auto collision = CollisionChecker::getFirstCollision(segment, geometryData, robotTrajectories);
            if (!collision) {
                break;
            }

            segment.startPos = collision->position;
            segment.startTime = collision->collisionTime;
            segment.startVel = collision->outVelocity;
            segment.acc = ekf.getAcc(); //TODO: remove dependency on ekf (when e.g. sliding is implemented)
            segment.dt = (segment.endTime - segment.startTime).asSeconds();
            segment.acceleration = collision->outVelocity.normalize() * ekf.getAcc();
            segment.endPos = segment.startPos + segment.startVel * segment.dt +
                             segment.acceleration * segment.dt * segment.dt * 0.5; //TODO: check if ball is lying still
            collisions.push_back(*collision);
            PredictedBall predictedBall;
            predictedBall.collisions = collisions;
            predictedBall.position = segment.endPos;
            predictedBall.time = time;
            predictedBalls.balls.push_back(predictedBall);
            ++i;
        }
    }
    return predictedBalls;
}


FilteredBall BallGroundFilter::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredBall ball;
    ball.pos = ekf.getPositionEstimate(time);
    ball.vel = ekf.getVelocityEstimate(time);
    ball.isVisible = time - lastSeen() > Time(0.05);
    if (writeUncertainties) {
        ball.health = getHealth();
        ball.posUncertainty = ekf.getPositionUncertainty().norm();
        ball.velocityUncertainty = ekf.getVelocityUncertainty().norm();
    }
    return ball;
}

bool BallGroundFilter::processDetections(const ObservationPredictionPair &opPair){
    //TODO: add collision detection from observations (not predictions) here to find robot kicks etc.
    predictFilter(opPair.prediction);
    bool removeFilter = false;
    if(!opPair.observation.has_value()){
        removeFilter = updateBallNotSeen(opPair.prediction.time);
    } else{
        update(opPair.observation.value());
    }
    return removeFilter;
}
void BallGroundFilter::predictFilter(const PredictedBall &prediction) {
    for(const auto& collision : prediction.collisions){
        ekf.predict(collision.collisionTime);
        //On a collision, reset the filters velocity estimate to that of the collision estimate and add some uncertainty
        //to the filters estimations to give new observations more weight
        //assert((collision.position-ekf.getPosition()).length()<(1e-5)); //TODO: figure out why this fails. Probably timing errors
        ekf.setVelocity(collision.outVelocity);
        ekf.addUncertainty(0.05,0.5);
    }
    ekf.predict(prediction.time);
}