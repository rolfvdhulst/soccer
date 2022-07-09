
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
        ball/BallFlyFilter.cpp
        include/visionFilter/ball/BallFlyFilter.h

        RobotOrientationFilter.cpp
        include/visionFilter/RobotOrientationFilter.h
        include/visionFilter/FilterConstants.h
#        ball/kick/estimation/chip/ChipEstimator6D.cpp
#        ball/kick/estimation/chip/ChipEstimator5D.cpp
#        ball/kick/estimation/chip/ChipEstimator.cpp
#        ball/kick/estimation/chip/ChipEstimator6DSingle.cpp
        ball/kick/estimation/chip/ChipEstimator6DRotated.cpp
        ObjectFilter.cpp include/visionFilter/ObjectFilter.h
        BallFilter.cpp include/visionFilter/BallFilter.h
        RobotFilter.cpp include/visionFilter/RobotFilter.h
        ball/BallGroundFilter.cpp
        include/visionFilter/ball/BallGroundFilter.h
        ball/collision/CollisionChecker.cpp
        include/visionFilter/ball/collision/CollisionChecker.h
        ball/BallTrajectorySegment.cpp
        include/visionFilter/ball/BallTrajectorySegment.h
        RobotTrajectorySegment.cpp
        include/visionFilter/RobotTrajectorySegment.h
        ball/VirtualBallTracker.cpp
        include/visionFilter/ball/VirtualBallTracker.h
        BallAssigment.cpp
        include/visionFilter/BallAssigment.h
        ball/BallGroundEKF.cpp
        ball/CameraBallFilter.cpp
        ball/kick/detection/KickEvent.cpp
        include/visionFilter/ball/kick/detection/KickEvent.h
        ball/kick/detection/KickTrajectory.cpp
        include/visionFilter/ball/kick/detection/KickTrajectory.h
        ball/kick/estimation/chip/ChipTrajectory.cpp
        include/visionFilter/ball/kick/estimation/chip/ChipTrajectory.h
        ball/kick/estimation/ground/GroundTrajectory.cpp
        include/visionFilter/ball/kick/estimation/ground/GroundTrajectory.h
        ball/kick/estimation/ground/GroundTwoPhaseEstimator.cpp
        include/visionFilter/ball/kick/estimation/ground/GroundTwoPhaseEstimator.h
        ball/kick/estimation/ground/GroundSlideEstimator.cpp
        include/visionFilter/ball/kick/estimation/ground/GroundSlideEstimator.h
        ball/kick/estimation/chip/ChipFitter.cpp
        ball/kick/estimation/chip/ChipFitResult.cpp
        ball/kick/detection/KickEventFilter.cpp
        include/visionFilter/ball/kick/detection/KickEventFilter.h
        ball/collision/VelocityChangeDetector.cpp
        include/visionFilter/ball/collision/VelocityChangeDetector.h
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