add_library(vision INTERFACE)

target_link_libraries(vision
        INTERFACE proto
        INTERFACE geometry
        INTERFACE core
        )
target_include_directories(vision
        INTERFACE include
)