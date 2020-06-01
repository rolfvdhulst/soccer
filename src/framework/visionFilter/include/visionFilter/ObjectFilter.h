//
// Created by rolf on 22-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_

#include <core/Time.h>

class ObjectFilter{
 public:
  ObjectFilter(double fullHealthToUnhealthyTime, double camFrameInterval, int successiveTicksMaxHealth, int healthyAfter);
  [[nodiscard]] const Time& lastSeen() const;
  [[nodiscard]] int observations() const;
  [[nodiscard]] double getHealth() const;
  [[nodiscard]] bool isHealthy() const;

 protected:
  void objectSeen(const Time& time);
  void objectInvisible(const Time& time);

 private:
  double health;
  const double INCREMENT;
  const double DECREMENT_SLOPE;
  const double MAXIMUM;
  const double HEALTHYLIMIT;
  int framesTotal = 0;
  Time lastSeenTime;
  Time lastUpdateTime;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
