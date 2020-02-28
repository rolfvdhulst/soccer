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
        Circle.cpp
        include/geometry/Circle.h
        Triangle.cpp
        include/geometry/Triangle.h
        Polygon.cpp
        include/geometry/Polygon.h
        Rectangle.cpp
        include/geometry/Rectangle.h
        Triangle.cpp
        include/geometry/Triangle.h
        Tube.cpp
        include/geometry/Tube.h
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