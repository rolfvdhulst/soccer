add_library(core STATIC
        Time.cpp
        include/core/Time.h
        Random.cpp include/core/Random.h)


target_include_directories(core
        INTERFACE include
        PRIVATE include/core
        )

target_compile_options(core
        PRIVATE -std=c++17
)