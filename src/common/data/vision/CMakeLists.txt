add_library(vision STATIC
        BallObservation.cpp
        RobotObservation.cpp
        DetectionFrame.cpp
        )

target_link_libraries(vision
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(vision
        INTERFACE include
        PRIVATE include/vision
)