//
// Created by rolf on 22-05-20.
//

#include <cmath>
#include <cassert>
#include <algorithm>
#include "ObjectFilter.h"

void ObjectFilter::objectSeen(const Time &time) {
    assert(time >= lastUpdateTime);
    double newHealth = health + INCREMENT - (time - lastUpdateTime).asSeconds() * DECREMENT_SLOPE;
    health = std::clamp(newHealth, 0.0, MAXIMUM);
    framesTotal++;
    lastSeenTime = time;
    lastUpdateTime = time;
}

void ObjectFilter::objectInvisible(const Time &time) {
    assert(time > lastSeenTime);
    health = fmax(health - (time - lastSeenTime).asSeconds() * DECREMENT_SLOPE, 0);
    lastUpdateTime = time;
}

double ObjectFilter::getHealth() const {
    return health;
}

int ObjectFilter::observations() const {
    return framesTotal;
}

bool ObjectFilter::isHealthy() const {
    return health >= HEALTHYLIMIT;
}

const Time &ObjectFilter::lastSeen() const {
    return lastSeenTime;
}

ObjectFilter::ObjectFilter(double fullHealthToUnhealthyTime, double camFrameInterval, int successiveTicksMaxHealth,
                           int healthyAfter, const Time& time) :
        MAXIMUM{100.0},
        HEALTHYLIMIT{100.0*double(healthyAfter) / double(successiveTicksMaxHealth)},
        DECREMENT_SLOPE{(100.0*(1- double(healthyAfter) / double(successiveTicksMaxHealth)))/ fullHealthToUnhealthyTime},
        INCREMENT{100.0/successiveTicksMaxHealth+ camFrameInterval*(100.0*(1- double(healthyAfter) / double(successiveTicksMaxHealth)))/ fullHealthToUnhealthyTime},
        lastSeenTime{time},
        lastUpdateTime{time}{
    health = INCREMENT;
}
