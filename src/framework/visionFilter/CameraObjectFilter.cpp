//
// Created by rolf on 22-05-20.
//

#include <cmath>
#include <cassert>
#include <algorithm>
#include "CameraObjectFilter.h"

#include <iostream>

void CameraObjectFilter::objectSeen(const Time &time) {
    assert(time >= lastUpdateTime);
    double newHealth = health + INCREMENT - (time - lastUpdateTime).asSeconds() * DECREMENT_SLOPE;
    health = std::clamp(newHealth, 0.0, MAXIMUM);
    ++framesTotal;
    lastSeenTime = time;
    lastUpdateTime = time;
    framesNotSeenFor = 0;
}

void CameraObjectFilter::objectInvisible(const Time &time) {
    assert(time >= lastSeenTime); //TODO: change back to >
    health = fmax(health - (time - lastSeenTime).asSeconds() * DECREMENT_SLOPE, 0);
    lastUpdateTime = time;
    ++framesNotSeenFor;
}

double CameraObjectFilter::getHealth() const {
    return health;
}

int CameraObjectFilter::observations() const {
    return framesTotal;
}

bool CameraObjectFilter::isHealthy() const {
    return health >= HEALTHYLIMIT;
}

const Time &CameraObjectFilter::lastSeen() const {
    return lastSeenTime;
}

CameraObjectFilter::CameraObjectFilter(double fullHealthToUnhealthyTime, double camFrameInterval, int successiveTicksMaxHealth,
                                       int healthyAfter, const Time& time) :
        MAXIMUM{100.0},
        HEALTHYLIMIT{100.0*double(healthyAfter) / double(successiveTicksMaxHealth)},
        DECREMENT_SLOPE{(100.0*(1- double(healthyAfter) / double(successiveTicksMaxHealth)))/ fullHealthToUnhealthyTime},
        INCREMENT{100.0/successiveTicksMaxHealth+ camFrameInterval*(100.0*(1- double(healthyAfter) / double(successiveTicksMaxHealth)))/ fullHealthToUnhealthyTime},
        lastSeenTime{time},
        lastUpdateTime{time}{
    health = INCREMENT;
}

void CameraObjectFilter::setID(int newId) {
    uniqueID = newId;
}
int CameraObjectFilter::getID() const {
    return uniqueID;
}

unsigned int CameraObjectFilter::consecutiveFramesNotSeen() const {
    return framesNotSeenFor;
}
