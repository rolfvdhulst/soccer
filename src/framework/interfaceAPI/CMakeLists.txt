add_library(interfaceAPI
        API.cpp
        include/interfaceAPI/API.h
        SettingsAPI.cpp include/interfaceAPI/SettingsAPI.h)
target_link_libraries(interfaceAPI
        PUBLIC proto
)
target_include_directories(interfaceAPI
        PRIVATE include/interfaceAPI
        INTERFACE include
        )