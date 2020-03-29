add_subdirectory(interface)
add_subdirectory(interfaceAPI)
add_subdirectory(net)
add_subdirectory(visionFilter)
add_subdirectory(ai)
add_library(application STATIC
        ApplicationManager.cpp
        ApplicationManager.h
        )
target_link_libraries(application
        PUBLIC net
        PUBLIC visionFilter
        PUBLIC interfaceAPI
        PUBLIC referee #TODO remove again
        )


add_executable(soccer
        main.cpp
        )
target_compile_options(soccer PRIVATE
        -g
        -O3
        -std=c++17
        -Wall
        )
target_link_libraries(soccer
        PRIVATE pthread
        PRIVATE application
        PRIVATE interface
)
