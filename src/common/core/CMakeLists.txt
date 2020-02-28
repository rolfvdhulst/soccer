add_library(core STATIC
        Time.cpp
        include/core/Time.h
        )


target_include_directories(core
        INTERFACE include
        PRIVATE include/core
        )