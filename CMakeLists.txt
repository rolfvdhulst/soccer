cmake_minimum_required(VERSION 3.10)
project(soccer)
add_subdirectory(src)
add_subdirectory(test)

#Code coverage
option(CODE_COVERAGE "Enable coverage reports" OFF)

function(target_add_coverage_flags TARGET)
    target_compile_options(${TARGET} PUBLIC
            -O0
            -g
            --coverage
            )
endfunction()

if (CODE_COVERAGE)
    #don't use  optimized code and set debug info, link coverage
    target_add_coverage_flags(allTests)
    target_add_coverage_flags(geometry)
    target_add_coverage_flags(core)
    target_add_coverage_flags(world)
    target_add_coverage_flags(field)
    target_add_coverage_flags(referee)
    target_add_coverage_flags(net)
    target_add_coverage_flags(application)
    target_compile_options(math INTERFACE
            -O0
            -g
            --coverage
            )
endif(CODE_COVERAGE)
