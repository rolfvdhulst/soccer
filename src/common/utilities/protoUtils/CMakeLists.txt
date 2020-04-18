add_library(protoUtils STATIC
        CommandSwitch.cpp
        Flip.cpp
        include/protoUtils/CommandSwitch.h
        include/protoUtils/Flip.h
        )
target_link_libraries(protoUtils
        PUBLIC proto
        PUBLIC math
        )
target_include_directories(protoUtils
        INTERFACE include
        PRIVATE include/protoUtils
        )