find_package(Eigen3 REQUIRED)
set(GEOMETRY_SOURCES
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
        include/math/geometry/Polygon.h
        geometry/Rectangle.cpp
        include/math/geometry/Rectangle.h
        geometry/Tube.cpp
        include/math/geometry/Tube.h
        geometry/Vector3.cpp
        include/math/geometry/Vector3.h
        geometry/LineBase.cpp
        include/math/geometry/LineBase.h
        include/math/geometry/Shape.h
        geometry/BoundingBox2D.cpp
        include/math/geometry/BoundingBox2D.h
        geometry/Ray.cpp
        include/math/geometry/Ray.h
        geometry/RobotShape.cpp
        include/math/geometry/RobotShape.h
        )
add_library(math STATIC
        ${GEOMETRY_SOURCES}
        filters/PosVelFilter2D.cpp
        include/math/filters/PosVelFilter2D.h
        filters/DEMAFilter.cpp
        include/math/filters/DEMAFilter.h
        include/math/filters/ConstantMAFilter.h
        general/QuadraticEquation.cpp
        include/math/general/QuadraticEquation.h
        )
target_link_libraries(math
        PRIVATE containers
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
