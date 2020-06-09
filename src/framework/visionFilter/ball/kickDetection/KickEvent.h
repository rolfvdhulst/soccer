//
// Created by rolf on 27-05-20.
//

#ifndef SOCCER_KICKEVENT_H
#define SOCCER_KICKEVENT_H

#include <math/geometry/Vector2.h>
#include <math/geometry/Vector3.h>
#include <core/Time.h>
enum KickType{
    STRAIGHT,
    CHIP
};
struct KickEvent{
    int kickID;
    bool kickBotIsBlue;
    Time kickTime;
    KickType type;
};
struct ChipKickEvent{
    Vector3 kickPos;
    Vector3 kickVel;
};
struct StraightKickEvent {
    Vector2 kickPos;
    Vector2 kickVel;

};


#endif //SOCCER_KICKEVENT_H
