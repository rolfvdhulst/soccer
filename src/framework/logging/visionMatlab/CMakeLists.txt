find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(visionMatlab STATIC
        VisionMatlabLogger.cpp
        )

target_link_libraries(visionMatlab
        PUBLIC Qt5::Core
        PUBLIC Eigen3::Eigen
        )
target_include_directories(visionMatlab
        INTERFACE include
        PRIVATE include/visionMatlab
        )
target_compile_options(visionMatlab
        PUBLIC -std=c++17
        )