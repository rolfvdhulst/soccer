//
// Created by rolf on 10-08-20.
//

#include "ball/CameraBallGroundEKF.h"

#include <utility>
#include <FilterConstants.h>
#include <visionMatlab/VisionMatlabLogger.h>
#include "collision/FieldWallCollisionChecker.h"

CameraBallGroundEKF::CameraBallGroundEKF(const BallObservation &observation, Eigen::Vector2d velocityEstimate) :
        CameraObjectFilter(0.2, 1 / 60.0, 15, 3, observation.timeCaptured) {
    Eigen::Vector4d startState = {observation.position.x(),observation.position.y(), velocityEstimate.x(), velocityEstimate.y()};

    Eigen::Matrix4d startCovariance = Eigen::Matrix4d::Zero();
    startCovariance(0,0) = BALL_POSITION_INITIAL_COV;
    startCovariance(1,1) = BALL_POSITION_INITIAL_COV;
    startCovariance(2,2) = BALL_VELOCITY_INITIAL_COV;
    startCovariance(3,3) = BALL_VELOCITY_INITIAL_COV;

    ekf = BallEKF(startState,startCovariance,BALL_POSITION_MODEL_ERROR,BALL_POSITION_MEASUREMENT_ERROR,observation.timeCaptured);
    registerLogFile(observation.position);

}

void CameraBallGroundEKF::registerLogFile(const Eigen::Vector2d &observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = matlab_logger::logger.writeNewFilter(4,2,VisionMatlabLogger::BALL_FILTER);
    setID(id);
    writeLogFile(observation);
}

void CameraBallGroundEKF::writeLogFile(const Eigen::Vector2d &observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = getID();
    if(id>0){
        matlab_logger::logger.writeData(
                id,
                ekf.lastUpdateTime.asSeconds(),
                observation,
                ekf.X,
                ekf.P,
                ekf.y
        );
    } else{
        registerLogFile(observation);
    }

}

void CameraBallGroundEKF::update(const BallObservation &observation) {
    ekf.update(observation.position);
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(observation.position);
}

bool CameraBallGroundEKF::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3;}

bool CameraBallGroundEKF::acceptObservation(const BallObservation &observation) const {
    return (observation.position-ekf.X.head<2>()).squaredNorm()<0.5*0.5;
}

Eigen::Vector2d CameraBallGroundEKF::getVelocity(const Time &time) const {
    double dt = fmin(0,(time-ekf.lastUpdateTime).asSeconds());
    Eigen::Vector2d vel = ekf.X.tail<2>();
    return vel+ekf.acc*vel.normalized()*dt;
}

void CameraBallGroundEKF::predict(Time time, const GeometryData& geometryData) {
    FieldWallCollisionChecker::SimpleBallSegment segment;
    segment.beforePos = Vector2(ekf.getPosition());
    segment.beforeTime = ekf.lastUpdateTime;
    segment.afterPos = Vector2(ekf.getPositionEstimate(time));
    segment.afterTime = time;
    segment.velocity = Vector2(ekf.getVelocity());
    //TODO: make sure collision detection and reflection calculation are seperated so we can pass velocity as a function of time
    if(! (segment.beforePos == segment.afterPos)) {
        auto collision = FieldWallCollisionChecker::getFieldOutsideWallCollision(segment, geometryData);
        if (collision) {
            double collisionVel = ekf.getVelocityEstimate(collision->collisionTime).norm();
            ekf.predict(collision->collisionTime);
            ekf.setVelocity(collision->outVelocity);
            ekf.addUncertainty(0.05,std::min(0.1,collisionVel*0.1));
            std::cout << "Collision at " << collision->ballCollisionPos << " filter state: "
                      << Vector2(ekf.getPosition())
                      << "vel: "<<collision->outVelocity
                      << std::endl;

        }
    }
    ekf.predict(time);

    lastCycleWasUpdate = false;
}

bool CameraBallGroundEKF::justUpdated() const {
    return lastCycleWasUpdate;
}

FilteredBall CameraBallGroundEKF::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredBall ball;
    ball.pos = ekf.getPositionEstimate(time);
    ball.vel = ekf.getVelocityEstimate(time);
    ball.isVisible = time-lastSeen()>Time(0.05);
    if(writeUncertainties){
        ball.health = getHealth();
        ball.posUncertainty = ekf.getPositionUncertainty().norm();
        ball.velocityUncertainty =ekf.getVelocityUncertainty().norm();
    }
    return ball;
}

void CameraBallGroundEKF::BallEKF::predict(const Time &timeStamp) {
    if(timeStamp<lastUpdateTime){
        std::__throw_invalid_argument("Bad timestamp");
        return;
    }
    double dt = (timeStamp-lastUpdateTime).asSeconds();
    if (dt<=0){
        return;
    }
    lastUpdateTime = timeStamp;

    Eigen::Vector2d currentPos = X.head<2>();
    Eigen::Vector2d currentVel = X.tail<2>();
    double vel = currentVel.norm();
    double velCubed = vel*vel*vel;
    //TODO: check if velocity reaches zero and put this into state model!
    X.head<2>() = currentPos + currentVel * dt + 0.5 * currentVel/vel * acc * dt * dt;
    X.tail<2>() = currentVel + currentVel/vel * acc * dt;


    double vysq = currentVel.y()*currentVel.y() /velCubed;
    double vxvy = -currentVel.x()*currentVel.y()/velCubed;
    double vxsq = currentVel.x()*currentVel.x() /velCubed;

    F(0,2) = dt + 0.5*vysq*acc*dt*dt;
    F(0,3) = 0.5*vxvy*acc*dt*dt;
    F(1,2) = 0.5*vxvy*acc*dt*dt;
    F(1,3) = dt + 0.5*vxsq*acc*dt*dt;
    F(2,2) = 1 + vysq*acc*dt;
    F(2,3) = vxvy*acc*dt;
    F(3,2) = vxvy*acc*dt;
    F(3,3) = 1+ vxsq*acc*dt;

    double sigma = modelError;
    double dt3 = (1.0 / 3.0) * dt * dt * dt * sigma * sigma;
    double dt2 = (1.0 / 2.0) * dt * dt * sigma * sigma;
    double dt1 = dt * sigma * sigma;

    Q(0,0) = dt3;
    Q(0,2) = dt2;
    Q(1,1) = dt3;
    Q(1,3) = dt2;

    Q(2,0) = dt2;
    Q(2,2) = dt1;
    Q(3,1) = dt2;
    Q(3,3) = dt1;

    P = F * P * F.transpose() + Q;

}

void CameraBallGroundEKF::BallEKF::update(const Eigen::Vector2d &observation) {
    // Compute innovation (error between measurement and state)
    y = observation - (H * X);
    // Variance of innovation
    Eigen::Matrix2d S = H * P * H.transpose() + R;
    // compute kalman gain. For small matrices, Eigen's inverse function is efficient but be careful with larger matrices (solve a system of eq's instead)
    Eigen::Matrix<double,4,2> K = P * H.transpose() * S.inverse();
    // update state with prediction
    X = X + K * y;
    // update covariance
    P -= K * H * P;
}

CameraBallGroundEKF::BallEKF::BallEKF(Eigen::Vector4d initialState, Eigen::Matrix4d initialCovariance,
                                      double modelError, double measurementError, const Time &timeStamp) :
                                      modelError{modelError},
                                      lastUpdateTime{timeStamp},
                                      X{std::move(initialState)},
                                      P{std::move(initialCovariance)},
                                      F{Eigen::Matrix4d::Identity()},
                                      H{Eigen::Matrix<double,2,4>::Identity()},
                                      Q{Eigen::Matrix4d::Zero()},
                                      R{Eigen::Matrix2d::Identity()*measurementError},
                                      y{Eigen::Vector2d::Zero()}
                                      { }

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getPosition() const {
    return X.head<2>();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getVelocityUncertainty() const {
    return P.diagonal().tail<2>().array().sqrt();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getVelocity() const {
    return X.tail<2>();
}

void CameraBallGroundEKF::BallEKF::setVelocity(const Eigen::Vector2d &velocity) {
    X.tail<2>() = velocity;
}

void CameraBallGroundEKF::BallEKF::addUncertainty(double posUncertainty, double velUncertainty) {
    P.diagonal().head<2>().array()+= posUncertainty;
    P.diagonal().tail<2>().array()+= velUncertainty;
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getPositionUncertainty() const {
    return P.diagonal().head<2>().array().sqrt();
}


