add_library(field STATIC
        FieldState.cpp
        Camera.cpp
        CameraMap.cpp
        include/field/GeometryData.h)

target_link_libraries(field
        PUBLIC geometry
        PUBLIC proto
        PUBLIC core
        )
target_include_directories(field
        INTERFACE include
        PRIVATE include/field
        )