add_library(world STATIC
        Robot.cpp
        include/world/Robot.h
        Ball.cpp
        include/world/Ball.h
        World.cpp
        include/world/World.h
        RobotID.cpp
        include/world/RobotID.h
        )


target_link_libraries(world
        PUBLIC geometry
        )
target_include_directories(world
        INTERFACE include
        PRIVATE include/world
        )
target_compile_features(world PUBLIC cxx_std_17)