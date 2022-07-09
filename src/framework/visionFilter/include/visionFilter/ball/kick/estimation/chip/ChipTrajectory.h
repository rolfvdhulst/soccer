//
// Created by rolf on 25-12-20.
//

#ifndef SOCCER_CHIPTRAJECTORY_H
#define SOCCER_CHIPTRAJECTORY_H

#include <core/Time.h>
#include <Eigen/Dense>

struct ChipBounceParameters{
    double restitution;
};

struct ChipState {
public:
    ChipState(Eigen::Vector3d position, Eigen::Vector3d velocity, Time time);
    static const Eigen::Vector3d gravity(){
        return Eigen::Vector3d(0,0,-9.81);
    };
    [[nodiscard]] ChipState prediction(Time time) const;
    [[nodiscard]] std::optional<ChipState> predictFutureBouncePosition() const;
    [[nodiscard]] std::optional<ChipState> predictWithBounces(Time time, ChipBounceParameters parameters) const;
    void reflectVelocity(ChipBounceParameters parameters, const Eigen::Vector3d& planeNormal = Eigen::Vector3d(0,0,1));
    [[nodiscard]] Eigen::Vector3d pos() const;
    Eigen::Vector3d vel() const;
    Time time() const;
private:
    Time current_time;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;

};


#endif //SOCCER_CHIPTRAJECTORY_H
