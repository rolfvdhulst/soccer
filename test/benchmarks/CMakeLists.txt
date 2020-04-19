find_package(benchmark REQUIRED)

add_executable(geometryBenchmarks
        filters/KalmanBenchmark.cpp
        geometryMain.cpp
        )

target_link_libraries(geometryBenchmarks
        PRIVATE math
        PRIVATE benchmark::benchmark
)
#Compile options are handled in main