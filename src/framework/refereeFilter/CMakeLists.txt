add_library(refereeFilter STATIC
        RefereeFilter.cpp
        include/refereeFilter/RefereeFilter.h
        )
target_link_libraries(refereeFilter
        PUBLIC proto
        PRIVATE geometry
        )
target_include_directories(refereeFilter
        PRIVATE include/refereeFilter
        INTERFACE include
        )
target_compile_options(refereeFilter PRIVATE
        -std=c++17
        )