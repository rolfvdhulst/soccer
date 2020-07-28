//
// Created by rolf on 28-07-20.
//

#ifndef SOCCER_VELOCITYCHANGEDETECTOR_H
#define SOCCER_VELOCITYCHANGEDETECTOR_H

#include <vision/BallObservation.h>
#include <containers/circular_buffer.h>

class VelocityChangeDetector {
public:
    bool detectKick(const BallObservation &observation);
private:
    bool velocityChanged() const;
    const Time COOLDOWN_TIME = Time(0.05);
    Time lastChangedTime = Time(-1.0);
    int observations = 0;
    circular_buffer<BallObservation,3> buffer;
};


#endif //SOCCER_VELOCITYCHANGEDETECTOR_H
