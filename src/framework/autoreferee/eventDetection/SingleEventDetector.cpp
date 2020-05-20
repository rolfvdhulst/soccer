//
// Created by rolf on 10-05-20.
//

#include "eventDetection/SingleEventDetector.h"
bool SingleEventDetector::canRetrigger(const Time &time) const {
  return (time-lastTriggerTime) >= reTriggerTime;
}
void SingleEventDetector::trigger(const Time &time) {
  lastTriggerTime = time;
}
