find_package(benchmark REQUIRED)

add_executable(kalmanBenchmarks
        filters/KalmanBenchmark.cpp
        kalmanMain.cpp
        )

target_link_libraries(kalmanBenchmarks
        PRIVATE math
        PRIVATE benchmark::benchmark
)

add_executable(geometryBenchmarks
        geometry/SegmentBenchmark.cpp
        kalmanMain.cpp
        )

target_link_libraries(geometryBenchmarks
        PRIVATE math
        PRIVATE benchmark::benchmark
        )
#Compile options are handled in main