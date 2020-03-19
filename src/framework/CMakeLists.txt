add_subdirectory(net)

add_library(application STATIC
        ApplicationManager.cpp
        ApplicationManager.h
        )
target_link_libraries(application
        PUBLIC net
        )


add_executable(soccer
        main.cpp
        )
target_compile_options(soccer PRIVATE
        -g
        -O0
        -std=c++17
        -Wall
        )

target_link_libraries(soccer
        PRIVATE pthread
        PRIVATE application
)
