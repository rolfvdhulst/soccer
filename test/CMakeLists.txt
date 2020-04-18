set(geometryTests
        common/geometry/LineTest.cpp
        common/geometry/RectangleTest.cpp
        common/geometry/TriangleTest.cpp
        common/geometry/TubeTest.cpp
        common/geometry/Vector2Test.cpp
        common/geometry/PolygonTest.cpp
        common/geometry/CircleTest.cpp
        common/geometry/AngleTest.cpp
        common/geometry/RotationTest.cpp
        )
set(coreTests
        common/core/TimeTest.cpp
        )
set(worldTests
        common/world/RobotIDTest.cpp
        )
set(mathTests
        common/math/KalmanFilterTest.cpp
        )
set(fieldTests
        common/field/CameraTest.cpp
        common/field/CameraMapTest.cpp common/field/FieldStateTest.cpp)
set(helperClasses
        helpers/ProtoExamples.h
        helpers/RandomWorldGeneration.cpp
        helpers/RandomWorldGeneration.h
        helpers/ProtoExamples.cpp)
add_executable(allTests
        main.cpp
        ${helperClasses}
        ${coreTests}
        ${geometryTests}
        ${mathTests}
        ${worldTests}
        ${fieldTests}
        )

target_link_libraries(allTests
        PRIVATE proto
        PRIVATE math
        PRIVATE core
        PRIVATE world
        PRIVATE field
        PRIVATE math
        PRIVATE gtest
        )
target_include_directories(allTests
        PRIVATE helpers
        )
target_compile_options(allTests PRIVATE
        -g
        -O0
        -std=c++17
        -Wall
        )

if (CODE_COVERAGE)
    target_link_libraries(allTests
            PRIVATE --coverage
            )
endif()