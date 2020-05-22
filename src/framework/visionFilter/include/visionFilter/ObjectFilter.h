//
// Created by rolf on 22-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_

class ObjectFilter{
 public:
  ObjectFilter(int increment, int decrement, int maximum);
  [[nodiscard]] int ticksNotSeenFor() const;
  [[nodiscard]] int observations() const;
  [[nodiscard]] int getHealth() const;

 protected:
  void objectSeen();
  void objectInvisible();

 private:
  int health = 0;
  const int INCREMENT;
  const int DECREMENT;
  const int MAXIMUM;
  int ticksIncreased = 0;
  int ticksNotSeen = 0;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_OBJECTFILTER_H_
