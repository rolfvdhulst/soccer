find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
set(CMAKE_AUTOMOC ON) #Instruct CMake to run moc automatically when needed

add_library(interface STATIC
        MainWindow.cpp
        include/interface/MainWindow.h
        Visualizer.cpp
        Visualizer.h
        )

target_link_libraries(interface
        PUBLIC Qt5::Core
        PUBLIC Qt5::Gui
        PUBLIC Qt5::Widgets
        PUBLIC proto
        PRIVATE geometry
        PUBLIC interfaceAPI
        PRIVATE field
)
target_include_directories(interface
        PRIVATE include/interface
        INTERFACE include
        )