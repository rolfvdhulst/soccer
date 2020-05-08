//
// Created by rolf on 20-04-20.
//

#include "Random.h"
#include "Time.h"
#include "iostream"

Random::Random(long seed) : engine{seed}{

}
Random::Random(const Time &time) :
  Random(time.asNanoSeconds()){
}
double Random::getGaussian() {
  //We could also instantiate these as class members
  std::normal_distribution<double> gauss(0.0,1.0);
  return gauss(engine);
}
std::mt19937 &Random::instance() {
  return engine;
}
double Random::getUniform() {
  std::uniform_real_distribution<double> uniform(0.0,1.0);
  return uniform(engine);
}
