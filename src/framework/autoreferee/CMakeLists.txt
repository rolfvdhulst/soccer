find_package(Qt5 COMPONENTS Network REQUIRED)
add_library(autoref
        eventDetection/EventDetector.cpp
        include/autoreferee/eventDetection/EventDetector.h
        AutoReferee.cpp
        include/autoreferee/AutoReferee.h
        network/GameControllerTcp.cpp
        network/GameControllerTcp.h
        eventDetection/SingleEventDetector.cpp include/autoreferee/eventDetection/SingleEventDetector.h include/autoreferee/eventDetection/Context.h eventDetection/BallPlacementDetector.cpp include/autoreferee/eventDetection/BallPlacementDetector.h eventDetection/BallPlacementInterferenceDetector.cpp include/autoreferee/eventDetection/BallPlacementInterferenceDetector.h)
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
        PUBLIC field
        PUBLIC world
        PUBLIC referee
        PUBLIC protoUtils
        )

add_executable(roboteam_autoref
        rtt_autoreferee.cpp
        )
target_link_libraries(roboteam_autoref
        PUBLIC autoref)
