//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTSTATE_H
#define SOCCER_ROBOTSTATE_H

#include <math/geometry/Angle.h>
#include <math/geometry/Rotation.h>
#include <math/geometry/Vector2.h>
#include "RobotID.h"

class RobotState {
    public:
        [[nodiscard]] const RobotID& id() const;
        [[nodiscard]] const Vector2& pos() const;
        [[nodiscard]] const Angle& angle() const;
        [[nodiscard]] const Vector2& vel() const;
        [[nodiscard]] const Rotation& angularVel() const;
    private:
        RobotID _id;
        Vector2 _position;
        Angle _angle;
        Vector2 _velocity;
        Rotation _angularVelocity;
};

#endif  // SOCCER_ROBOTSTATE_H
