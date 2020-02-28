find_package(Protobuf 3.0.0 REQUIRED)




protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        proto/Vector2f.proto
        )

add_library(protobuf SHARED
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        )

target_link_libraries(protobuf
        PUBLIC ${PROTOBUF_LIBRARIES}
        )
target_include_directories(protobuf
        PUBLIC ${PROTOBUF_INCLUDE_DIRS}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..
        )