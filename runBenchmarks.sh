rm -rf build
mkdir build
cd build
cmake -DBENCHMARK=ON ..
make math geometryBenchmarks -j6
./test/benchmarks/geometryBenchmarks --benchmark_repetitions=10 --benchmark_report_aggregates_only=true
