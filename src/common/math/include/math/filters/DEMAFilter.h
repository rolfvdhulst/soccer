//
// Created by rolf on 20-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_FILTERS_DEMAFILTER_H_
#define SOCCER_SRC_COMMON_MATH_FILTERS_DEMAFILTER_H_
#include "math/geometry/Vector2.h"
/**
 * A simple Discrete Exponential Moving Average Filter:
 * Uses transfer function x(k+1) = a*x(k) + (1-a)*z(k),
 * where x(k) is the state of the filter, and z(k) is the observation at iteration k.
 */
class DEMAFilter {
 public:
  explicit DEMAFilter(double alpha, double initialValue);
  /**
   * @brief update filter with new measurement
   * @param measurement
   * @return state after measurement
   */
   double update(double measurement);
   [[nodiscard]] double getAlpha() const;
   void setAlpha(double alpha);
   [[nodiscard]] double getState() const;
 private:
  double alpha;
  double state;
};
class DEMAFilter2D {
 public:
  explicit DEMAFilter2D(double alpha, const Vector2& initialValue);
  /**
   * @brief update filter with new measurement
   * @param measurement
   * @return state after measurement
   */
  Vector2 update(const Vector2& measurement);
  [[nodiscard]] double getAlpha() const;
  void setAlpha(double alpha);
  [[nodiscard]] Vector2 getState() const;
 private:
  double alpha;
  Vector2 state;
};

#endif //SOCCER_SRC_COMMON_MATH_FILTERS_DEMAFILTER_H_
