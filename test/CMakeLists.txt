SET(geometryTests
        common/geometry/LineTest.cpp
        common/geometry/RectangleTest.cpp
        common/geometry/TriangleTest.cpp
        common/geometry/TubeTest.cpp
        common/geometry/Vector2Test.cpp
        )

add_executable(allTests
        main.cpp
        ${geometryTests}
        )

target_link_libraries(allTests
        PRIVATE geometry
        PRIVATE gtest
        )