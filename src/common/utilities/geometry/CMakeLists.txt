find_package(Eigen3 REQUIRED)

add_library(geometry STATIC
        Vector2.cpp
        include/geometry/Vector2.h
        Angle.cpp
        include/geometry/Angle.h
        include/geometry/Definitions.h
        Rotation.cpp
        include/geometry/Rotation.h
        LineSegment.cpp
        include/geometry/LineSegment.h
        Line.cpp
        include/geometry/Line.h
        )
target_link_libraries(geometry
        PUBLIC protobuf
        PUBLIC Eigen3::Eigen
        )
target_include_directories(geometry
        INTERFACE include
        PRIVATE include/geometry
        )
target_compile_features(geometry PUBLIC cxx_std_17)