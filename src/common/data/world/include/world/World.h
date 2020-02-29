//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_WORLD_H
#define SOCCER_WORLD_H

#include <optional>
#include "Ball.h"
#include "Robot.h"

class World {
   private:
    // TimePoint time;
    std::optional<Ball> ball;
    std::vector<Robot> us;
    std::vector<Robot> them;
};

#endif  // SOCCER_WORLD_H
