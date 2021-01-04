//
// Created by rolf on 25-12-20.
//

#include "ball/kick/estimation/chip/ChipTrajectory.h"
#include <math/general/QuadraticEquation.h>

void ChipState::reflectVelocity(ChipBounceParameters parameters, const Eigen::Vector3d& planeNormal) {
    Eigen::Vector3d normalized_normal = planeNormal.normalized();
    Eigen::Vector3d reflection = velocity - 2*(velocity*normalized_normal)*normalized_normal;
    velocity = reflection*parameters.restitution;
}

ChipState ChipState::prediction(Time at_time) const {
    double dt = (at_time-time).asSeconds();
    Eigen::Vector3d pred_pos = position + velocity*dt + 0.5*gravity()*dt*dt;
    Eigen::Vector3d pred_vel = velocity + gravity()*dt;
    return ChipState(pred_pos,pred_vel,at_time);
}
ChipState::ChipState(Eigen::Vector3d pos, Eigen::Vector3d vel, Time time) :position(std::move(pos)), velocity(std::move(vel)), time(time)
{
}

std::optional<ChipState> ChipState::predictFutureBouncePosition() const {
    const double ball_radius = 0.021333; //TODO: collect somewhere
    //-gt^2+v*t+p_z = ball_radius
    auto collisionTime =  solveQuadratic(gravity().z()*0.5,velocity.z(),position.z()-ball_radius);
    if(collisionTime){
        double largestTime = fmax(collisionTime->first,collisionTime->second);
        return prediction(Time(largestTime));
    }
    return std::nullopt;
}

std::optional<ChipState> ChipState::predictWithBounces(Time at_time, ChipBounceParameters parameters) const{
    int nrBounces = 0;
    const int MAXBOUNCES=10;
    ChipState predicted_state = *this;
    while( nrBounces < MAXBOUNCES){
        auto bounce_state= predicted_state.predictFutureBouncePosition();
        if(bounce_state){
            if(bounce_state->time < at_time){
                predicted_state = *bounce_state;
                predicted_state.reflectVelocity(parameters);
                ++nrBounces;
            }else{
                return predicted_state.prediction(at_time);
            }
        }else{
            //Ball does not bounce, already below the ground... let us return nullopt
            return std::nullopt;
        }
    }
    //After MAXBOUNCES, we assume the ball is probably not flying anymore. We simply return the last bounce position at this point, but this shouldnt happen much.
    return predicted_state;
}

