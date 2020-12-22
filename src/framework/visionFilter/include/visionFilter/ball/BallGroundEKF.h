//
// Created by rolf on 21-12-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_BALLGROUNDEKF_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_BALLGROUNDEKF_H_

#include <Eigen/Dense>
#include <core/Time.h>
#include "BallTrajectorySegment.h"

class BallGroundEKF {
 public:
  BallGroundEKF() = default;
  BallGroundEKF(Eigen::Vector4d initialState,Eigen::Matrix4d initialCovariance,
          double modelError, double measurementError, const Time& timeStamp);
  void predict(const Time& predictionTime);
  void update(const Eigen::Vector2d& observation);
  //TODO: use the below functions in the appropriate places
  [[nodiscard]] Eigen::Vector2d getPosition() const;
  [[nodiscard]] Eigen::Vector2d getPositionEstimate(const Time& time) const;
  [[nodiscard]] Eigen::Vector4d getStateEstimate(const Time& time) const;
  [[nodiscard]] Eigen::Vector4d getStateEstimate(double dt) const;
  [[nodiscard]] Eigen::Vector2d getVelocity() const;
  [[nodiscard]] Eigen::Vector2d getVelocityEstimate(const Time& time) const;
  void setVelocity(const Eigen::Vector2d& velocity);
  void addUncertainty(double posUncertainty, double velUncertainty);
  //TODO: make these time-based?
  [[nodiscard]] Eigen::Vector2d getVelocityUncertainty() const;
  [[nodiscard]] Eigen::Vector2d getPositionUncertainty() const;
  [[nodiscard]] double getAcc() const;
  void setAcc(double acceleration);

  [[nodiscard]] Eigen::Vector2d innovation() const;
  [[nodiscard]] Time lastUpdated() const;

  [[nodiscard]] Eigen::Vector4d state() const;
  [[nodiscard]] Eigen::Matrix4d covariance() const;

  [[nodiscard]] BallTrajectorySegment getSegment(Time time) const;
 private:
  double modelError;
  double acc = - 0.3;
  Time lastUpdateTime;
  Eigen::Vector4d X;
  Eigen::Matrix4d P;

  Eigen::Matrix4d F;    // Forward model/state update matrix
  Eigen::Matrix<double, 2, 4> H;   // Observation model/ states how we can interpret observation as our state
  Eigen::Matrix4d Q;    // Covariance of the process noise. (Amount of "Random Forces" we can expect in the process)
  Eigen::Matrix2d R;  // Observation Noise Covariance. Keeps track of how noisy the observations are.

  Eigen::Vector2d y; //Innovation. Not strictly necessary to store but often used to measure performance

  void setProccessNoise(double dt);
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_BALLGROUNDEKF_H_
