//
// Created by rolf on 22-05-20.
//

#include <algorithm>
#include "ObjectFilter.h"

ObjectFilter::ObjectFilter(int increment, int decrement, int maximum, int healthyLimit) :
INCREMENT(increment),
DECREMENT(decrement),
MAXIMUM(maximum),
HEALTHYLIMIT(healthyLimit){}
void ObjectFilter::objectSeen() {
  health = std::min(health+INCREMENT,MAXIMUM);
  ticksNotSeen = 0;
  ticksIncreased ++;
}
void ObjectFilter::objectInvisible() {
  health = std::max(health-DECREMENT,0);
  ticksNotSeen ++;
}
int ObjectFilter::getHealth() const{
  return health;
}
int ObjectFilter::ticksNotSeenFor() const {
  return ticksNotSeen;
}
int ObjectFilter::observations() const {
  return ticksIncreased;
}
bool ObjectFilter::isHealthy() const {
  return health >= HEALTHYLIMIT;
}
ObjectFilter::ObjectFilter(int increment, int decrement, int maximum) :
ObjectFilter(increment,decrement,maximum,maximum){ }
