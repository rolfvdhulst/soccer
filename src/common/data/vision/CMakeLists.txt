add_library(vision STATIC
        BallObservation.cpp
        RobotObservation.cpp
        DetectionFrame.cpp
        FilteredBall.cpp
        include/vision/FilteredBall.h
        FilteredRobot.cpp
        include/vision/FilteredRobot.h
        FilteredVisionFrame.cpp
        include/vision/FilteredVisionFrame.h
        )

target_link_libraries(vision
        PUBLIC math
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(vision
        INTERFACE include
        PRIVATE include/vision
)