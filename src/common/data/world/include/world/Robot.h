//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOT_H
#define SOCCER_ROBOT_H

#include <geometry/Vector2.h>
#include <geometry/Angle.h>
#include "RobotID.h"

class Robot {
    private:
        RobotID id;
        Vector2 position;
        Angle angle;
        Vector2 velocity;
        //Rotation angularVelocity
};

#endif //SOCCER_ROBOT_H
