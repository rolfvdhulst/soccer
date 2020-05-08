//
// Created by rolf on 20-04-20.
//

#ifndef SOCCER_SRC_COMMON_CORE_RANDOM_H_
#define SOCCER_SRC_COMMON_CORE_RANDOM_H_

#include <random>
class Time;
/**
 * @brief This class generates random numbers. The seed is provided by the user.
 * This is to generate deterministic sequences. Note that for using replayability,
 * using Time::now() is pretty much always a bad idea.
 */
class Random {
 public:
  explicit Random(long seed);
  explicit Random(const Time& time);
  /**
   * @return a gaussian distributed number with gaussian parameters mean 0, standard deviation 1
   */
  double getGaussian();
  /**
   * @return a uniformly distributed random number on the [0,1] interval
   */
  double getUniform();
  std::mt19937& instance();//Can be used for custom distributions

 private:
  std::mt19937 engine;

};

#endif //SOCCER_SRC_COMMON_CORE_RANDOM_H_
