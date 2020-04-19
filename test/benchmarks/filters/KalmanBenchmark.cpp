//
// Created by rolf on 19-04-20.
//
#include <benchmark/benchmark.h>
#include <math/filters/KalmanFilter.h>

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
// Register the function as a benchmark
BENCHMARK(predictTime);
BENCHMARK(updateTime);
BENCHMARK(totalTime);

