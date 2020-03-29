add_library(refereeFilter STATIC
        RefereeFilter.cpp
        include/refereeFilter/RefereeFilter.h
        )
target_link_libraries(refereeFilter
        PUBLIC proto
        )
target_include_directories(refereeFilter
        PRIVATE include/refereeFilter
        INTERFACE include
        )