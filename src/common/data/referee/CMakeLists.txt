add_library(referee STATIC
        GameStage.cpp
        include/referee/GameStage.h
        )


target_link_libraries(referee
        PUBLIC geometry
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(referee
        INTERFACE include
        PRIVATE include/referee
        )