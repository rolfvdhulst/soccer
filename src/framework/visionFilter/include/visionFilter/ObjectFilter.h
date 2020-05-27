//
// Created by rolf on 22-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_

#include <core/Time.h>

class ObjectFilter{
 public:
  ObjectFilter(int increment, int decrement, int maximum);
  ObjectFilter(int increment, int decrement, int maximum, int healthyLimit);
  [[nodiscard]] int ticksNotSeenFor() const;
  [[nodiscard]] int observations() const;
  [[nodiscard]] int getHealth() const;
  [[nodiscard]] bool isHealthy() const;

 protected:
  void objectSeen(const Time& time);
  void objectInvisible();

 private:
  int health = 0;
  const int INCREMENT;
  const int DECREMENT;
  const int MAXIMUM;
  const int HEALTHYLIMIT;
  int ticksIncreased = 0;
  int ticksNotSeen = 0;
  Time lastSeenTime;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
