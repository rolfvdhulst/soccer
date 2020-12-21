
add_library(visionFilter
        VisionFilter.cpp
        include/visionFilter/VisionFilter.h
        GeometryFilter.cpp
        include/visionFilter/GeometryFilter.h
        CameraRobotFilter.cpp
        include/visionFilter/CameraRobotFilter.h
        WorldFilter.cpp
        include/visionFilter/WorldFilter.h
        CameraObjectFilter.cpp
        include/visionFilter/CameraObjectFilter.h
        ball/BallGroundFilter.cpp
        include/visionFilter/ball/BallGroundFilter.h
        ball/BallFlyFilter.cpp
        include/visionFilter/ball/BallFlyFilter.h
        ball/stateEstimation/BallStateEstimator.cpp
        ball/stateEstimation/BallStateEstimator.h
        ball/kickDetection/KickEvent.cpp
        ball/kickDetection/KickEvent.h
        RobotOrientationFilter.cpp
        include/visionFilter/RobotOrientationFilter.h
        include/visionFilter/FilterConstants.h
        ball/kickEstimation/chip/ChipEstimator6D.cpp
        ball/kickEstimation/chip/ChipEstimator6D.h
        ball/kickEstimation/chip/ChipEstimator5D.cpp
        ball/kickEstimation/chip/ChipEstimator5D.h
        ball/kickEstimation/chip/ChipEstimator.cpp
        ball/kickEstimation/chip/ChipEstimator.h
        ball/kickEstimation/chip/ChipEstimator6DSingle.cpp
        ball/kickEstimation/chip/ChipEstimator6DSingle.h
        ball/kickEstimation/chip/ChipEstimator6DRotated.cpp
        ball/kickEstimation/chip/ChipEstimator6DRotated.h
        ball/kickDetection/VelocityChangeDetector.cpp
        ball/kickDetection/VelocityChangeDetector.h
        ball/kickDetection/VelocityChangeEvent.cpp
        ball/kickDetection/VelocityChangeEvent.h
        ball/kickDetection/WallCollisionDetector.cpp
        ball/kickDetection/WallCollisionDetector.h
        ball/kickDetection/RobotCollisionDetector.cpp
        ball/kickDetection/RobotCollisionDetector.h
        ball/kickDetection/CollisionEvent.cpp
        ball/kickDetection/CollisionEvent.h
        ball/kickDetection/AreaHeightDetector.cpp
        ball/kickDetection/AreaHeightDetector.h
        ball/kickDetection/InreasingDistanceDetector.cpp
        ball/kickDetection/InreasingDistanceDetector.h
        ball/kickDetection/InFrontDetector.cpp
        ball/kickDetection/InFrontDetector.h
        ObjectFilter.cpp include/visionFilter/ObjectFilter.h
        BallFilter.cpp include/visionFilter/BallFilter.h
        RobotFilter.cpp include/visionFilter/RobotFilter.h
        ball/CameraBallGroundEKF.cpp
        include/visionFilter/ball/CameraBallGroundEKF.h
        ball/collision/CollisionChecker.cpp
        include/visionFilter/ball/collision/CollisionChecker.h
        ball/collision/CollisionCalculations.cpp
        ball/collision/CollisionCalculations.h
        ball/BallTrajectorySegment.cpp
        include/visionFilter/ball/BallTrajectorySegment.h
        RobotTrajectorySegment.cpp
        include/visionFilter/RobotTrajectorySegment.h
        ball/VirtualBallTracker.cpp
        include/visionFilter/ball/VirtualBallTracker.h
        BallAssigment.cpp
        include/visionFilter/BallAssigment.h
        )
target_link_libraries(visionFilter
        PRIVATE math
        PRIVATE core
        PUBLIC containers
        PUBLIC world
        PUBLIC visionMatlab
        PUBLIC vision
        PUBLIC field
        PUBLIC proto

        )
target_include_directories(visionFilter
        PUBLIC include/visionFilter #TODO: fix include to private?
        INTERFACE include
        )