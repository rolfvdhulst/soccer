add_library(visionFilter
        VisionFilter.cpp
        include/visionFilter/VisionFilter.h
        GeometryFilter.cpp
        include/visionFilter/GeometryFilter.h
        ball/BallFilter.cpp
        include/visionFilter/ball/BallFilter.h
        CameraFilter.cpp
        include/visionFilter/CameraFilter.h
        RobotFilter.cpp
        include/visionFilter/RobotFilter.h
        include/visionFilter/Scaling.h
        WorldFilter.cpp
        include/visionFilter/WorldFilter.h
        ObjectFilter.cpp
        include/visionFilter/ObjectFilter.h
        ball/BallGroundFilter.cpp
        include/visionFilter/ball/BallGroundFilter.h
        ball/BallFlyFilter.cpp
        include/visionFilter/ball/BallFlyFilter.h
        ball/stateEstimation/BallStateEstimator.cpp
        ball/stateEstimation/BallStateEstimator.h
        ball/kickDetection/KickDetector.cpp
        ball/kickDetection/KickDetector.h
        ball/kickDetection/KickEvent.cpp
        ball/kickDetection/KickEvent.h
        RobotOrientationFilter.cpp
        include/visionFilter/RobotOrientationFilter.h
        include/visionFilter/FilterConstants.h
        )
target_link_libraries(visionFilter
        PRIVATE math
        PRIVATE core
        PRIVATE containers
        PUBLIC visionMatlab
        PUBLIC vision
        PUBLIC field
        PUBLIC proto

        )
target_include_directories(visionFilter
        PUBLIC include/visionFilter #TODO: fix include to private?
        INTERFACE include
        )