//
// Created by rolf on 21-12-20.
//

#include "ball/BallGroundEKF.h"

void BallGroundEKF::update(const Eigen::Vector2d &observation) {
  // Compute innovation (error between measurement and state)
  y = observation - (H * X);
  // Variance of innovation
  Eigen::Matrix2d S = H * P * H.transpose() + R;
  // compute kalman gain. For small matrices, Eigen's inverse function is efficient but be careful with larger matrices (solve a system of eq's instead)
  Eigen::Matrix<double, 4, 2> K = P * H.transpose() * S.inverse();
  // update state with prediction
  X = X + K * y;
  // update covariance
  P -= K * H * P;
}

BallGroundEKF::BallGroundEKF(Eigen::Vector4d initialState, Eigen::Matrix4d initialCovariance,
                                      double modelError, double measurementError, const Time &timeStamp) :
    modelError{modelError},
    lastUpdateTime{timeStamp},
    X{std::move(initialState)},
    P{std::move(initialCovariance)},
    F{Eigen::Matrix4d::Identity()},
    H{Eigen::Matrix<double, 2, 4>::Identity()},
    Q{Eigen::Matrix4d::Zero()},
    R{Eigen::Matrix2d::Identity() * measurementError},
    y{Eigen::Vector2d::Zero()} {}

Eigen::Vector2d BallGroundEKF::getPosition() const {
  return X.head<2>();
}

Eigen::Vector2d BallGroundEKF::getVelocityUncertainty() const {
  return P.diagonal().tail<2>().array().sqrt();
}

Eigen::Vector2d BallGroundEKF::getVelocity() const {
  return X.tail<2>();
}

void BallGroundEKF::setVelocity(const Eigen::Vector2d &velocity) {
  X.tail<2>() = velocity;
}

void BallGroundEKF::addUncertainty(double posUncertainty, double velUncertainty) {
  P.diagonal().head<2>().array() += posUncertainty;
  P.diagonal().tail<2>().array() += velUncertainty;
}

Eigen::Vector2d BallGroundEKF::getPositionUncertainty() const {
  return P.diagonal().head<2>().array().sqrt();
}

double BallGroundEKF::getAcc() const {
  return acc;
}

void BallGroundEKF::setAcc(double acceleration) {
  acc = acceleration;
}

void BallGroundEKF::setProccessNoise(double dt) {
  double sigma = modelError;
  double dt3 = (1.0 / 3.0) * dt * dt * dt * sigma * sigma;
  double dt2 = (1.0 / 2.0) * dt * dt * sigma * sigma;
  double dt1 = dt * sigma * sigma;

  Q(0, 0) = dt3;
  Q(0, 2) = dt2;
  Q(1, 1) = dt3;
  Q(1, 3) = dt2;

  Q(2, 0) = dt2;
  Q(2, 2) = dt1;
  Q(3, 1) = dt2;
  Q(3, 3) = dt1;
}

Eigen::Vector4d BallGroundEKF::getStateEstimate(double dt) const {
  Eigen::Vector2d currentPos = getPosition();
  Eigen::Vector2d currentVel = getVelocity();
  double vel = currentVel.norm();
  Eigen::Vector4d estimate;
  if (vel != 0.0) {
    estimate.head<2>() = currentPos + currentVel * dt + 0.5 * currentVel / vel * acc * dt * dt;
    estimate.tail<2>() = currentVel + currentVel / vel * acc * dt;
  } else {
    estimate.head<2>() = currentPos;
    estimate.tail<2>() = currentVel;
  }

  return estimate;
}

Eigen::Vector4d BallGroundEKF::getStateEstimate(const Time &time) const {
  if (time < lastUpdateTime) {
    std::cout << "Too late by: " << (lastUpdateTime - time).asMilliSeconds() << " ms" << std::endl;
    std::__throw_invalid_argument("Bad timestamp");
  }
  double frame_dt = (time - lastUpdateTime).asSeconds();
  if (frame_dt <= 0) {
    return X;
  }
  double vel = getVelocity().norm();
  //We need to check if the velocity does not reach 0, as at that point the ball simply lays still
  //Because of the dimples it usually lays still when it reaches ~= 0.01 cm/s but this is typically negligible
  double dt = frame_dt;
  if (vel + acc * frame_dt < 0) {
    dt = -vel / acc;
  }
  return getStateEstimate(dt);
}

Eigen::Vector2d BallGroundEKF::getVelocityEstimate(const Time &time) const {
  return getStateEstimate(time).tail<2>();
}

Eigen::Vector2d BallGroundEKF::getPositionEstimate(const Time &time) const {
  return getStateEstimate(time).head<2>();
}

Eigen::Vector2d BallGroundEKF::innovation() const {
  return y;
}

Time BallGroundEKF::lastUpdated() const {
  return lastUpdateTime;
}

Eigen::Vector4d BallGroundEKF::state() const {
  return X;
}

Eigen::Matrix4d BallGroundEKF::covariance() const {
  return P;
}

BallTrajectorySegment BallGroundEKF::getSegment(Time time) const {
  BallTrajectorySegment segment;
  segment.startPos = Vector2(getPosition());
  segment.startTime = lastUpdated();
  segment.endPos = Vector2(getPositionEstimate(time));
  segment.endTime = time;
  segment.startVel = Vector2(getVelocity());
  segment.acc = getAcc();
  segment.dt = (segment.endTime - segment.startTime).asSeconds();
  segment.acceleration = Vector2(getVelocity()).normalize() * getAcc(); //TODO: check sign
  return segment;
}

void BallGroundEKF::predict(const Time &timeStamp) {
  if (timeStamp < lastUpdateTime) {
    std::__throw_invalid_argument("Bad timestamp");
    return;
  }
  double frame_dt = (timeStamp - lastUpdateTime).asSeconds();
  if (frame_dt <= 0) {
    return;
  }
  lastUpdateTime = timeStamp;

  Eigen::Vector2d currentPos = getPosition();
  Eigen::Vector2d currentVel = getVelocity();
  double vel = currentVel.norm();
  //We need to check if the velocity does not reach 0, as at that point the ball simply lays still
  //Because of the dimples it usually lays still when it reaches ~= 0.01 cm/s but this is typically negligible
  double dt = frame_dt;
  if (vel + acc * frame_dt < 0) {
    dt = -vel / acc;
  }


  double velCubed = vel * vel * vel;
  double vysq = currentVel.y() * currentVel.y() / velCubed;
  double vxvy = -currentVel.x() * currentVel.y() / velCubed;
  double vxsq = currentVel.x() * currentVel.x() / velCubed;
  if (vel == 0.0) {
    vxsq = 0.0;
    vxvy = 0.0;
    vysq = 0.0;
  }


  F(0, 2) = dt + 0.5 * vysq * acc * dt * dt;
  F(0, 3) = 0.5 * vxvy * acc * dt * dt;
  F(1, 2) = 0.5 * vxvy * acc * dt * dt;
  F(1, 3) = dt + 0.5 * vxsq * acc * dt * dt;
  F(2, 2) = 1 + vysq * acc * dt;
  F(2, 3) = vxvy * acc * dt;
  F(3, 2) = vxvy * acc * dt;
  F(3, 3) = 1 + vxsq * acc * dt;

  setProccessNoise(frame_dt);
  if (vel != 0.0) {
    X.head<2>() = currentPos + currentVel * dt + 0.5 * currentVel / vel * acc * dt * dt;
    X.tail<2>() = currentVel + currentVel / vel * acc * dt;

  }
  P = F * P * F.transpose() + Q;

}
