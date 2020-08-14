//
// Created by rolf on 10-08-20.
//

#include "ball/CameraBallGroundEKF.h"

#include <utility>

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

    X = F * X;
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

void CameraBallGroundEKF::BallEKF::predictAndSetJacobian(double dt) {


}
