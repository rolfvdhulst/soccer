find_package(Qt5 COMPONENTS Network REQUIRED)
add_library(autoref
        eventDetection/EventDetector.cpp
        eventDetection/EventDetector.h
        AutoReferee.cpp
        include/autoreferee/AutoReferee.h
        network/GameControllerTcp.cpp
        network/GameControllerTcp.h
        )
target_compile_options(autoref PUBLIC
        -std=c++17
        )
target_include_directories(autoref
        PRIVATE include/autoreferee
        PUBLIC include
)
target_link_libraries(autoref
        PUBLIC Qt5::Network
        PRIVATE proto
        PRIVATE core
        PUBLIC net
        PUBLIC visionFilter
        PUBLIC refereeFilter
        )

add_executable(roboteam_autoref
        rtt_autoreferee.cpp
        )
target_link_libraries(roboteam_autoref
        PUBLIC autoref)
