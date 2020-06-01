//
// Created by rolf on 22-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_

#include <core/Time.h>

class ObjectFilter{
 public:
    //we need to define move assignment operator
  ObjectFilter(double fullHealthToUnhealthyTime, double camFrameInterval, int successiveTicksMaxHealth, int healthyAfter,const Time& time);
  [[nodiscard]] const Time& lastSeen() const;
  [[nodiscard]] int observations() const;
  [[nodiscard]] double getHealth() const;
  [[nodiscard]] bool isHealthy() const;

 protected:
  void objectSeen(const Time& time);
  void objectInvisible(const Time& time);

 private:
  double health;
  //We made these 4 non-const because this makes it possible to use some STL algorithms making things a LOT easier to program.
  //Note they should be constant
  double INCREMENT;
  double DECREMENT_SLOPE;
  double MAXIMUM;
  double HEALTHYLIMIT;
  int framesTotal = 0;
  Time lastSeenTime;
  Time lastUpdateTime;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
