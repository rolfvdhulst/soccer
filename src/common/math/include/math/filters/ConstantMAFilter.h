//
// Created by rolf on 22-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_FILTERS_CONSTANTMAFILTER_H_
#define SOCCER_SRC_COMMON_MATH_FILTERS_CONSTANTMAFILTER_H_
#include <containers/circular_buffer.h>
#include "math/geometry/Vector2.h"
template<std::size_t _Size>
class ConstantMAFilter {
 public:
  ConstantMAFilter() : sum{0}{  }
  double update(double measurement){
    if(valueQueue.full()){
      sum-=valueQueue.front();
    }
    valueQueue.push_back(measurement);
    sum+=measurement;
    return getState();
  }

  [[nodiscard]] double getState() const{
    return sum / valueQueue.size();
  }
  /**
   * @return the number of used measurements as of now
   */
  [[nodiscard]] std::size_t usedMeasurements() const{
    return valueQueue.size();
  }
 private:
  circular_buffer<double,_Size> valueQueue;
  double sum;
};
template<std::size_t _Size>
class ConstantMAFilter2D {
 public:
  ConstantMAFilter2D() : sum{Vector2()}{  }
  Vector2 update(Vector2 measurement){
    if(valueQueue.full()){
      sum-=valueQueue.front();
    }
    valueQueue.push_back(measurement);
    sum+=measurement;
    return getState();
  }

  [[nodiscard]] Vector2 getState() const{
    return sum / valueQueue.size();
  }
  /**
   * @return the number of used measurements as of now
   */
  [[nodiscard]] std::size_t usedMeasurements() const{
    return valueQueue.size();
  }
 private:
  circular_buffer<Vector2,_Size> valueQueue;
  Vector2 sum;
};

#endif //SOCCER_SRC_COMMON_MATH_FILTERS_CONSTANTMAFILTER_H_
