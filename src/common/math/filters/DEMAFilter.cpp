//
// Created by rolf on 20-04-20.
//

#include "filters/DEMAFilter.h"
DEMAFilter::DEMAFilter(double alpha, double initialValue) : 
alpha{alpha},
state{initialValue}{
  assert(alpha>=0 && alpha <=1); //assert that alpha value is correct
}
double DEMAFilter::update(double measurement) {
  state = alpha*state + (1-alpha)*measurement;
  return state;
}
double DEMAFilter::getAlpha() const {
  return alpha;
}
void DEMAFilter::setAlpha(double newAlpha) {
  alpha = newAlpha;
}
double DEMAFilter::getState() const {
  return state;
}
DEMAFilter2D::DEMAFilter2D(double alpha, const Vector2& initialValue) :
alpha{alpha},
state{initialValue}{
  assert(alpha>=0 && alpha <=1); //assert that alpha value is correct
}
Vector2 DEMAFilter2D::update(const Vector2& measurement) {
  state = state*alpha + measurement*(1-alpha);
  return state;
}
double DEMAFilter2D::getAlpha() const {
  return alpha;
}
void DEMAFilter2D::setAlpha(double newAlpha) {
  alpha = newAlpha;
}
Vector2 DEMAFilter2D::getState() const {
  return state;
}