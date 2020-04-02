find_package(Qt5 COMPONENTS Core REQUIRED)
add_library(visionLogger STATIC
        VisionLogHeader.h
        include/visionLogger/MessageType.h
        VisionLogReader.cpp
        include/visionLogger/VisionLogReader.h
        VisionLogWriter.cpp include/visionLogger/VisionLogWriter.h)

target_link_libraries(visionLogger
        PUBLIC proto
        PUBLIC Qt5::Core
        )
target_include_directories(visionLogger
        INTERFACE include
        PRIVATE include/visionLogger
        )
target_compile_options(visionLogger
        PUBLIC -std=c++17
        )