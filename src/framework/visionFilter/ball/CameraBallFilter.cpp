//
// Created by rolf on 17-11-19.
//

#include "ball/CameraBallFilter.h"
#include "FilterConstants.h"
#include <visionMatlab/VisionMatlabLogger.h>

CameraBallFilter::CameraBallFilter(const BallObservation& observation, Eigen::Vector2d velocityEstimate)  :
        CameraObjectFilter(0.2, 1 / 60.0, 15, 3, observation.timeCaptured)
{

    Eigen::Vector4d startState = {observation.position.x(),observation.position.y(), velocityEstimate.x(), velocityEstimate.y()};

    Eigen::Matrix4d startCovariance = Eigen::Matrix4d::Zero();
    startCovariance(0,0) = BALL_POSITION_INITIAL_COV;
    startCovariance(1,1) = BALL_POSITION_INITIAL_COV;
    startCovariance(2,2) = BALL_VELOCITY_INITIAL_COV;
    startCovariance(3,3) = BALL_VELOCITY_INITIAL_COV;

    positionFilter = PosVelFilter2D(startState,startCovariance,BALL_POSITION_MODEL_ERROR,BALL_POSITION_MEASUREMENT_ERROR,observation.timeCaptured);
    registerLogFile(observation.position);
}

bool CameraBallFilter::justUpdated() const {
    return lastCycleWasUpdate;
}

void CameraBallFilter::predict(Time time) {
    positionFilter.predict(time);
    lastCycleWasUpdate = false;
}
void CameraBallFilter::update(const BallObservation &observation) {
    positionFilter.update(observation.position);
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(observation.position);
}

bool CameraBallFilter::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3;
}

FilteredBall CameraBallFilter::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredBall ball;
    ball.pos = positionFilter.getPositionEstimate(time);
    ball.vel = positionFilter.getVelocity();
    ball.isVisible = time-lastSeen()>Time(0.05);
    if(writeUncertainties){
        ball.health = getHealth();
        ball.posUncertainty = positionFilter.getPositionUncertainty().norm();
        ball.velocityUncertainty = positionFilter.getVelocityUncertainty().norm();
    }
    return ball;
}

void CameraBallFilter::writeLogFile(const Eigen::Vector2d& observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = getID();
    if(id>0){
        matlab_logger::logger.writeData(
                id,
                positionFilter.lastUpdated().asSeconds(),
                observation,
                positionFilter.getState(),
                positionFilter.getCovariance(),
                positionFilter.getInnovation()
        );
    } else{
        registerLogFile(observation);
    }

}
void CameraBallFilter::registerLogFile(const Eigen::Vector2d &observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = matlab_logger::logger.writeNewFilter(4,2,VisionMatlabLogger::BALL_FILTER);
    setID(id);
    writeLogFile(observation);
}

bool CameraBallFilter::acceptObservation(const BallObservation &observation) const {
    return (observation.position-positionFilter.getPosition()).squaredNorm()<0.5*0.5;
}

Eigen::Vector2d CameraBallFilter::getVelocity(const Time &time) const {
    //TODO: two phase ball model.
    return positionFilter.getVelocity();
}
