//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_BALLSTATE_H
#define SOCCER_BALLSTATE_H

#include <core/Time.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
class BallState {
   private:
    Vector2 position;
    Vector2 velocity;
    bool isVisible;
};
class GroundBallState {
   private:
    Vector2 position;
    Vector2 velocity;
    bool isVisible;
};
class GroundKickData {
   private:
    Time kickTime;
    Vector2 kickPos;
    Vector2 kickVel;
};
class AirBallState {
   private:
    Vector3 position;
    Vector3 velocity;
    Vector2 predictedBouncePosition;
    Time predictedBounceTime;
};

#endif  // SOCCER_BALLSTATE_H
