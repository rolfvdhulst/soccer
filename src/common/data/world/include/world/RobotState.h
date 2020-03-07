//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTSTATE_H
#define SOCCER_ROBOTSTATE_H

#include <geometry/Angle.h>
#include <geometry/Rotation.h>
#include <geometry/Vector2.h>
#include "RobotID.h"

class RobotState {
    private:
        RobotID id;
        Vector2 position;
        Angle angle;
        Vector2 velocity;
        Rotation angularVelocity;
};

#endif  // SOCCER_ROBOTSTATE_H
