cmake_minimum_required(VERSION 3.10)
project(soccer)
add_subdirectory(src)
add_subdirectory(test)


#Code coverage
option(CODE_COVERAGE "Enable coverage reports" OFF)
option(RELEASE "add release flags" OFF)

function(target_add_coverage_flags TARGET)
    target_compile_options(${TARGET} PUBLIC
            -O0
            -g
            --coverage
            )
endfunction()

if (CODE_COVERAGE)
    #don't use  optimized code and set debug info, link coverage
    #We only do this for all targets which we want coverage on.
    target_add_coverage_flags(allTests)
    target_add_coverage_flags(geometry)
    target_add_coverage_flags(core)
    target_add_coverage_flags(world)
    target_add_coverage_flags(field)
    target_add_coverage_flags(referee)
    target_add_coverage_flags(vision)
    target_add_coverage_flags(application)
    target_add_coverage_flags(visionFilter)
    target_add_coverage_flags(refereeFilter)
    target_compile_options(math INTERFACE
            -O0
            -g
            --coverage
            )
endif (CODE_COVERAGE)

function(target_add_release_flags TARGET)
    target_compile_options(${TARGET} PUBLIC
            -O2
            -flto
            )
endfunction()
if (RELEASE)
    target_add_release_flags(proto)
    target_add_release_flags(geometry)
    target_add_release_flags(core)
    target_add_release_flags(world)
    target_add_release_flags(field)
    target_add_release_flags(referee)
    target_add_release_flags(vision)
    target_add_release_flags(net)
    target_add_release_flags(application)
    target_add_release_flags(visionFilter)
    target_add_release_flags(refereeFilter)
    target_add_release_flags(soccer)
endif (RELEASE)