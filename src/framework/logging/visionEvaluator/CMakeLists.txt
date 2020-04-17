find_package(Qt5 COMPONENTS Core REQUIRED)
add_executable(visionEvaluator
        visionEvaluator.cpp
)
target_link_libraries(visionEvaluator
        PRIVATE Qt5::Core
        PRIVATE visionLogger
        PRIVATE visionFilter
        )
