add_library(math INTERFACE)

target_include_directories(geometry
        INTERFACE include
        PRIVATE include/math
        )