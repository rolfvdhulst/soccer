add_library(ai STATIC
        AI.cpp
        include/ai/AI.h
        )

#target_link_libraries(ai
#
 #       )
target_include_directories(ai
        PRIVATE include/ai
        INTERFACE include
        )