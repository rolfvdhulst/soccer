find_package(Protobuf 3.0.0 REQUIRED)




protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        proto/Vector2f.proto
        proto/messages_robocup_ssl_detection.proto
        proto/messages_robocup_ssl_geometry.proto
        proto/messages_robocup_ssl_geometry_legacy.proto
        proto/messages_robocup_ssl_wrapper.proto
        proto/messages_robocup_ssl_wrapper_legacy.proto
        proto/ssl_game_controller_common.proto
        proto/ssl_game_controller_team.proto
        proto/ssl_game_controller_autoref.proto
        proto/ssl_game_event.proto
        proto/ssl_referee.proto
        proto/World.proto
        proto/WorldBall.proto
        proto/WorldRobot.proto
        proto/WorldVirtualBall.proto
        proto/RobotInfo.proto
        proto/GameState.proto
        proto/FrameLog.proto
        proto/Settings.proto
        proto/RobotCommand.proto
        proto/AISettings.proto
        proto/DebugInfo.proto
        )

add_library(proto
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        RobotInfo.cpp
        include/protobuf/RobotInfo.h)


target_link_libraries(proto
        PUBLIC ${PROTOBUF_LIBRARIES}
        )
target_include_directories(proto
        PUBLIC ${PROTOBUF_INCLUDE_DIRS}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..
        PRIVATE include/protobuf
        INTERFACE include
        )