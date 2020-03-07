//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_BALLSTATE_H
#define SOCCER_BALLSTATE_H

#include <geometry/Vector2.h>
class BallState {
    private:
        Vector2 position;
        Vector2 velocity;
        bool isVisible;
};

#endif  // SOCCER_BALLSTATE_H
