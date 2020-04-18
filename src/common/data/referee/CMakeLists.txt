add_library(referee STATIC
        GameStage.cpp
        include/referee/GameStage.h
        GameState.cpp
        include/referee/GameState.h
        GameCommand.cpp
        include/referee/GameCommand.h
        GameEvent.cpp
        include/referee/GameEvent.h
        Team.cpp
        include/referee/Team.h
        TeamInfo.cpp
        include/referee/TeamInfo.h)


target_link_libraries(referee
        PUBLIC math
        PUBLIC proto
        PUBLIC world
        PUBLIC core
        )
target_include_directories(referee
        INTERFACE include
        PRIVATE include/referee
        )