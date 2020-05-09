add_library(ai STATIC
        AiBase.cpp include/ai/AiBase.h)

target_link_libraries(ai
        PUBLIC proto
        )

target_include_directories(ai
        PRIVATE include/ai
        INTERFACE include
        )
target_compile_features(ai PUBLIC cxx_std_17)