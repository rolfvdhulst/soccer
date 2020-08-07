//
// Created by rolf on 29-07-20.
//

#include "VelocityChangeEvent.h"

#include <utility>

VelocityChangeEvent::VelocityChangeEvent(BallObservation first, BallObservation second,
                                         BallObservation third) :
                                         first{std::move(first)},
                                         second{std::move(second)},
                                         third{std::move(third)}{
}
