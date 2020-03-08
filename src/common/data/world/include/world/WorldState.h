//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_WORLDSTATE_H
#define SOCCER_WORLDSTATE_H

#include <optional>
#include "BallState.h"
#include "RobotState.h"
#include <core/Time.h>

class WorldState {
    private:
        Time time;
        std::optional<BallState> ball;
        std::vector<RobotState> us;
        std::vector<RobotState> them;
};

#endif  // SOCCER_WORLDSTATE_H
