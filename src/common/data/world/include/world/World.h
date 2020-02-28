//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_WORLD_H
#define SOCCER_WORLD_H

#include "Ball.h"
#include "Robot.h"
#include <optional>

class World {
    private:
        //TimePoint time;
        std::optional<Ball> ball;
        std::vector<Robot> us;
        std::vector<Robot> them;
};

#endif //SOCCER_WORLD_H
