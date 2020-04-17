add_subdirectory(interface)
add_subdirectory(interfaceAPI)
add_subdirectory(net)
add_subdirectory(refereeFilter)
add_subdirectory(visionFilter)
add_subdirectory(ai)
add_subdirectory(logging)

add_library(application STATIC
        ApplicationManager.cpp
        ApplicationManager.h
        )
target_link_libraries(application
        PUBLIC net
        PUBLIC visionFilter
        PUBLIC interfaceAPI
        PUBLIC refereeFilter
        PUBLIC logger
        )


add_executable(soccer
        main.cpp
        )
target_compile_options(soccer PUBLIC
        -g
        -O0
        -std=c++17
        -Wall
        )
target_link_libraries(soccer
        PRIVATE pthread
        PRIVATE application
        PRIVATE interface
)
