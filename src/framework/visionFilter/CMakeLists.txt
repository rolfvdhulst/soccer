add_library(visionFilter
        VisionFilter.cpp
        include/visionFilter/VisionFilter.h
        GeometryFilter.cpp
        include/visionFilter/GeometryFilter.h
        BallFilter.cpp
        include/visionFilter/BallFilter.h
        CameraFilter.cpp
        include/visionFilter/CameraFilter.h
        RobotFilter.cpp
        include/visionFilter/RobotFilter.h
        include/visionFilter/Scaling.h
        WorldFilter.cpp
        include/visionFilter/WorldFilter.h
        )
target_link_libraries(visionFilter
        PRIVATE math
        PUBLIC vision
        PRIVATE core
        PUBLIC field
        PUBLIC proto
        )
target_include_directories(visionFilter
        PRIVATE include/visionFilter
        INTERFACE include
        )