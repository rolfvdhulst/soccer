add_library(referee STATIC
        GameStage.cpp
        include/referee/GameStage.h
        GameState.cpp include/referee/GameState.h GameStageMessage.cpp include/referee/GameStageMessage.h GameCommand.cpp include/referee/GameCommand.h)


target_link_libraries(referee
        PUBLIC geometry
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(referee
        INTERFACE include
        PRIVATE include/referee
        )