//
// Created by rolf on 19-04-20.
//
#include <benchmark/benchmark.h>
#include <math/filters/KalmanFilter.h>
#include <math/filters/PosVelFilter2D.h>
#include <core/Random.h>

//easily adjust the dimensions of the KF's tested here
#define S 4
#define O 2

static void predictTime(benchmark::State& state) {
  KalmanFilter<S, O>::Vector initialGuess = KalmanFilter<S, O>::Vector::Zero();
  KalmanFilter<S, O>::Matrix initalCovar = KalmanFilter<S, O>::Matrix::Identity();
  KalmanFilter<S, O> filter1(initialGuess, initalCovar);


  std::vector<KalmanFilter<S,O>::VectorO> zVector;
  for (int kI = 0; kI < 100; ++kI) {
    zVector.push_back(KalmanFilter<S,O>::VectorO::Random());
  }
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (const auto& z : zVector) {
      filter1.predict();
    }
    KalmanFilter<S,O>::Vector kState =filter1.state();

    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}
static void updateTime(benchmark::State& state) {
  KalmanFilter<S, O>::Vector initialGuess = KalmanFilter<S, O>::Vector::Zero();
  KalmanFilter<S, O>::Matrix initalCovar = KalmanFilter<S, O>::Matrix::Identity();
  KalmanFilter<S, O> filter1(initialGuess, initalCovar);


  std::vector<KalmanFilter<S,O>::VectorO> zVector;
  for (int kI = 0; kI < 100; ++kI) {
    zVector.push_back(KalmanFilter<S,O>::VectorO::Random());
  }
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (const auto& z : zVector) {
      filter1.update(z);
    }
    KalmanFilter<S,O>::Vector kState =filter1.state();

    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}
static void totalTime(benchmark::State& state){
  KalmanFilter<S, O>::Vector initialGuess = KalmanFilter<S, O>::Vector::Zero();
  KalmanFilter<S, O>::Matrix initalCovar = KalmanFilter<S, O>::Matrix::Identity();
  KalmanFilter<S, O> filter1(initialGuess, initalCovar);


  std::vector<KalmanFilter<S,O>::VectorO> zVector;
  for (int kI = 0; kI < 100; ++kI) {
    zVector.push_back(KalmanFilter<S,O>::VectorO::Random());
  }
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for (const auto& z : zVector) {
      filter1.predict();
      filter1.update(z);
    }
    KalmanFilter<S,O>::Vector kState =filter1.state();

    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}

std::vector<std::pair<Time,Eigen::Vector2d>> samples(){
  int numSamples = 101;
  double velocity = 3.0;
  Random gen(57);
  std::vector<std::pair<Time,Eigen::Vector2d>> measurements;
  for (int i = 0; i < numSamples; i++) {
    Time time(0.01*i);
    double posX = time.asSeconds()*velocity + gen.getGaussian()*0.02; //2 cm noise
    double posY = time.asSeconds()*velocity +gen.getGaussian()*0.02; //
    measurements.push_back(std::make_pair(time,Eigen::Vector2d(posX,posY)));
  }
  return measurements;
}
static void predictTimePosVelFilter(benchmark::State& state) {
  Eigen::Vector4d initialState = Eigen::Vector4d::Zero();

  auto measurements = samples();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 0.01, Time());
    for (const auto& z : measurements) {
      filter.predict(z.first);
    }
    Eigen::Vector2d kState = filter.getPosition();
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}
static void updateTimePosVelFilter(benchmark::State& state) {
  Eigen::Vector4d initialState = Eigen::Vector4d::Zero();

  auto measurements = samples();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 0.01, Time());
    for (const auto& z : measurements) {
      filter.update(z.second);
    }
    Eigen::Vector2d kState = filter.getPosition();
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}
static void totalTimePosVelFilter(benchmark::State& state) {
  Eigen::Vector4d initialState = Eigen::Vector4d::Zero();

  auto measurements = samples();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 0.01, Time());
    for (const auto& z : measurements) {
      filter.predict(z.first);
      filter.update(z.second);
    }
    Eigen::Vector2d kState = filter.getPosition();
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(kState);
  }
}
// Register the function as a benchmark
//BENCHMARK(predictTime);
//BENCHMARK(updateTime);
BENCHMARK(totalTime);

//BENCHMARK(predictTimePosVelFilter);
//BENCHMARK(updateTimePosVelFilter);
BENCHMARK(totalTimePosVelFilter);