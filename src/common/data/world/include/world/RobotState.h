//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTSTATE_H
#define SOCCER_ROBOTSTATE_H

#include <math/geometry/Angle.h>
#include <math/geometry/Rotation.h>
#include <math/geometry/Vector2.h>
#include <math/geometry/RobotShape.h>
#include <protobuf/WorldRobot.pb.h>
#include "RobotID.h"
#include "RobotParameters.h"

class RobotState {
    public:
        explicit RobotState(const proto::WorldRobot &protoRobot, const proto::RobotInfo &info);
        explicit RobotState(const proto::WorldRobot &protoRobot, RobotParameters parameters);
        [[nodiscard]] const RobotID& id() const;
        [[nodiscard]] const Vector2& pos() const;
        [[nodiscard]] const Angle& angle() const;
        [[nodiscard]] const Vector2& vel() const;
        [[nodiscard]] const Rotation& angularVel() const;
        [[nodiscard]] const RobotShape& shape() const;
        [[nodiscard]] double radius() const;
        [[nodiscard]] double centerToFront() const;
        [[nodiscard]] const RobotParameters& parameters() const;
    private:
        RobotID _id;
        Vector2 _position;
        Angle _angle;
        Vector2 _velocity;
        Rotation _angularVelocity;
        RobotParameters _parameters;
        RobotShape _shape;
};

#endif  // SOCCER_ROBOTSTATE_H
