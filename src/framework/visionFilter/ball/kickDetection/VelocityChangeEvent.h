//
// Created by rolf on 29-07-20.
//

#ifndef SOCCER_VELOCITYCHANGEEVENT_H
#define SOCCER_VELOCITYCHANGEEVENT_H

#include <vision/BallObservation.h>
class VelocityChangeEvent {
public:
    VelocityChangeEvent(BallObservation  first, BallObservation  second, BallObservation  third);
    BallObservation first;
    BallObservation second;
    BallObservation third;

};


#endif //SOCCER_VELOCITYCHANGEEVENT_H
