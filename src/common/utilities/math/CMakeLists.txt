add_library(math INTERFACE)
target_link_libraries(math
        INTERFACE Eigen3::Eigen
        )
target_include_directories(geometry
        INTERFACE include
        PRIVATE include/math
        )