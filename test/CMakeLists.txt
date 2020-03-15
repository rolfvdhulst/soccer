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
add_executable(allTests
        main.cpp
        ${geometryTests}
        ${coreTests}
        ${worldTests}
        )

target_link_libraries(allTests
        PRIVATE geometry
        PRIVATE core
        PRIVATE world
        PRIVATE gtest
        )
target_compile_options(allTests PRIVATE
        -g
        -O0
        -std=c++17
        -Wall
        )

option(CODE_COVERAGE "Enable coverage reports" OFF)

function(target_add_coverage_flags TARGET)
    target_compile_options(${TARGET} PRIVATE
            -O0
            -g
            --coverage
            )
endfunction()

if (CODE_COVERAGE)
    #don't use  optimized code and set debug info, link coverage
    target_link_libraries(allTests
            PRIVATE --coverage
            )
    target_add_coverage_flags(allTests)
    target_add_coverage_flags(geometry)
    target_add_coverage_flags(core)
    target_add_coverage_flags(world)
endif(CODE_COVERAGE)