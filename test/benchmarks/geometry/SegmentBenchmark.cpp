//
// Created by rolf on 25-04-20.
//
#include <benchmark/benchmark.h>
#include <math/geometry/LineSegment.h>
#include <core/Random.h>
#include <core/Time.h>
static std::vector<std::pair<LineSegment,LineSegment>> samples(){
  Random random(Time::now());
  std::vector<std::pair<LineSegment,LineSegment>> linePairs;
  int count = 0;
  while (count != 1000) {
    std::pair<LineSegment,LineSegment> lines =   std::make_pair(LineSegment(Vector2(random.getUniform(),random.getUniform()),Vector2(random.getUniform(),random.getUniform())),
                             LineSegment(Vector2(random.getUniform(),random.getUniform()),Vector2(random.getUniform(),random.getUniform())));
    if(lines.first.intersects(lines.second)!=std::nullopt){
      count ++;
      linePairs.push_back(lines);
    }
  }
  return linePairs;
}

static std::pair<LineSegment,LineSegment> sample(){

  Random random(Time::now());
  std::pair<LineSegment,LineSegment> lines;
  while(true){
    lines =   std::make_pair(LineSegment(Vector2(random.getUniform(),random.getUniform()),Vector2(random.getUniform(),random.getUniform())),
                             LineSegment(Vector2(random.getUniform(),random.getUniform()),Vector2(random.getUniform(),random.getUniform())));
    if(lines.first.intersects(lines.second)!=std::nullopt){
      return lines;
    }
  }
}
static void segmentSegmentIntersectionSingle(benchmark::State& state) {
  auto pair = sample();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    std::optional<Vector2> value=pair.first.intersects(pair.second);
    Vector2 out = *value;
    benchmark::DoNotOptimize(out);
  }
}
static void segmentSegmentBaseSingle(benchmark::State& state) {
  auto pair = sample();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    Vector2 value = pair.first.start();
    benchmark::DoNotOptimize(value);
  }
}
static void segmentSegmentIntersectionMultiple(benchmark::State& state) {
  std::vector<std::pair<LineSegment,LineSegment>> pairs = samples();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for(const auto& pair : pairs){
      std::optional<Vector2> value=pair.first.intersects(pair.second);
      Vector2 out = *value;
      benchmark::DoNotOptimize(out);
    }
  }
}
static void segmentSegmentBaseMultiple(benchmark::State& state) {
  std::vector<std::pair<LineSegment,LineSegment>> pairs = samples();
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    for(const auto& pair : pairs){
      Vector2 out = pair.first.start();
      benchmark::DoNotOptimize(out);
    }
  }
}
BENCHMARK(segmentSegmentIntersectionSingle);
BENCHMARK(segmentSegmentBaseSingle);
BENCHMARK(segmentSegmentIntersectionMultiple);
BENCHMARK(segmentSegmentBaseMultiple);