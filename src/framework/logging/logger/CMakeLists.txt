find_package(Qt5 COMPONENTS Core REQUIRED)
add_library(logger STATIC
        LogReader.cpp
        include/logger/LogReader.h
        LogWriter.cpp
        include/logger/LogWriter.h
        LogHeader.h LogCreator.cpp include/logger/LogCreator.h)

target_link_libraries(logger
        PUBLIC proto
        PUBLIC Qt5::Core
        PRIVATE core
        )
target_include_directories(logger
        INTERFACE include
        PRIVATE include/logger
        )
target_compile_options(logger
        PUBLIC -std=c++17
        )