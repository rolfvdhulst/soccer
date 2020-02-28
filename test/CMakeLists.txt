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

option(CODE_COVERAGE "Enable coverage reports" OFF)
if (CODE_COVERAGE)
    #don't use  optimized code and set debug info, link coverage
    target_compile_options(allTests
            -0
            -g
            --coverage
            )
    target_link_libraries(allTests
            PRIVATE --coverage
            )
endif(CODE_COVERAGE)