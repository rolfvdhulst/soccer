add_library(world STATIC
        RobotState.cpp
        include/world/RobotState.h
        BallState.cpp
        include/world/BallState.h
        WorldState.cpp
        include/world/WorldState.h
        RobotID.cpp
        include/world/RobotID.h
        include/world/WorldTeam.h)


target_link_libraries(world
        PUBLIC math
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(world
        INTERFACE include
        PRIVATE include/world
        )
