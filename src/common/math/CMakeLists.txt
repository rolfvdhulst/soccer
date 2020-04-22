find_package(Eigen3 REQUIRED)
SET(GEOMETRY_SOURCES
        geometry/Vector2.cpp
        include/math/geometry/Vector2.h
        geometry/Angle.cpp
        include/math/geometry/Angle.h
        include/math/geometry/Definitions.h
        geometry/Rotation.cpp
        include/math/geometry/Rotation.h
        geometry/LineSegment.cpp
        include/math/geometry/LineSegment.h
        geometry/Line.cpp
        include/math/geometry/Line.h
        geometry/Circle.cpp
        include/math/geometry/Circle.h
        geometry/Triangle.cpp
        include/math/geometry/Triangle.h
        geometry/Polygon.cpp
        include/math/geometry/Polygon.h
        geometry/Rectangle.cpp
        include/math/geometry/Rectangle.h
        geometry/Triangle.cpp
        include/math/geometry/Triangle.h
        geometry/Tube.cpp
        include/math/geometry/Tube.h
        geometry/Vector3.cpp
        include/math/geometry/Vector3.h
        )
add_library(math STATIC
        ${GEOMETRY_SOURCES}
        filters/PosVelFilter2D.cpp
        include/math/filters/PosVelFilter2D.h
        filters/DEMAFilter.cpp
        include/math/filters/DEMAFilter.h
        )
target_link_libraries(math
        PUBLIC proto
        PUBLIC Eigen3::Eigen
        PUBLIC core
        )

target_include_directories(math
        INTERFACE include
        PRIVATE include/
        PRIVATE include/math
        )
target_compile_features(math PUBLIC cxx_std_17)
