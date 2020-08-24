//
// Created by rolf on 11-08-20.
//

#include "ball/CameraBallFilter_v2.h"
#include "FilterConstants.h"
#include <visionMatlab/VisionMatlabLogger.h>
#include "collision/FieldWallCollisionChecker.h"

CameraBallFilter_v2::CameraBallFilter_v2(const BallObservation& observation, Eigen::Vector2d velocityEstimate)  :
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

void CameraBallFilter_v2::predict(const Time& time, const GeometryData& geometryData ) {
    FieldWallCollisionChecker::SimpleBallSegment segment;
    segment.beforePos = Vector2(positionFilter.getPosition());
    segment.beforeTime = positionFilter.lastUpdated();
    segment.afterPos = Vector2(positionFilter.getPositionEstimate(time));
    segment.afterTime = time;
    segment.velocity = Vector2(positionFilter.getVelocity());
    if(! (segment.beforePos == segment.afterPos)) {
        auto collision = FieldWallCollisionChecker::getFieldOutsideWallCollision(segment, geometryData);
        if (collision) {
            double collisionVel = positionFilter.getVelocity().norm();
            positionFilter.predict(collision->collisionTime);
            positionFilter.setVelocity(collision->outVelocity);
            positionFilter.addUncertainty(0.05,std::min(0.1,collisionVel*0.1));
            Eigen::Vector2d posUnc = positionFilter.getPositionUncertainty();
            Eigen::Vector2d velUnc = positionFilter.getVelocityUncertainty();
            std::cout << "Collision at " << collision->ballCollisionPos << " filter state: "
                      << Vector2(positionFilter.getPosition())
                      << "vel: "<<collision->outVelocity
                      << std::endl;

        }
    }
    positionFilter.predict(time);

    lastCycleWasUpdate = false;
}
void CameraBallFilter_v2::update(const BallObservation &observation) {
    positionFilter.update(observation.position);
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(observation.position);
}

bool CameraBallFilter_v2::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3;
}

FilteredBall CameraBallFilter_v2::getEstimate(const Time &time, bool writeUncertainties) const {
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

void CameraBallFilter_v2::writeLogFile(const Eigen::Vector2d& observation) {
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
void CameraBallFilter_v2::registerLogFile(const Eigen::Vector2d &observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = matlab_logger::logger.writeNewFilter(4,2,VisionMatlabLogger::BALL_FILTER);
    setID(id);
    writeLogFile(observation);
}

bool CameraBallFilter_v2::acceptObservation(const BallObservation &observation) const {
    return (observation.position-positionFilter.getPosition()).squaredNorm()<0.5*0.5;
}

Eigen::Vector2d CameraBallFilter_v2::getVelocity(const Time &time) const {
    return positionFilter.getVelocity();
}

bool CameraBallFilter_v2::addObservation(const BallObservation &observation) {
    bool accepted = acceptObservation(observation);
    if(accepted){
        lastFrameObservations.push_back(observation);
    }
    return accepted;
}

bool CameraBallFilter_v2::processFrame() {
    bool removeFilter = false;
    if(lastFrameObservations.empty()){
        removeFilter = updateBallNotSeen(positionFilter.lastUpdated());
    }else if(lastFrameObservations.size()==1){
        update(lastFrameObservations.at(0));
    }else{
        //more than one observation. There are a few cases this can happen:
        //1: The ball is seen as two separate balls.
        //2: A robot on the frame was also detected as a ball.
        //3: (unlikely): there is more than one ball on the field
        BallObservation merged = mergeBallObservationsByArea(lastFrameObservations);
        update(merged);
    }
    lastFrameObservations.clear();
    return removeFilter;
}
