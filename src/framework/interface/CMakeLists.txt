find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
set(CMAKE_AUTOMOC ON) #Instruct CMake to run moc automatically when needed

add_library(interface STATIC
        MainWindow.cpp
        include/interface/MainWindow.h
        Visualizer.cpp
        Visualizer.h
        GameStateVisualizer.cpp
        GameStateVisualizer.h
        GameEventsWidget.cpp
        GameEventsWidget.h
        MainSettingsWidget.cpp
        MainSettingsWidget.h
        MainTeamSettingsWidget.cpp
        MainTeamSettingsWidget.h
        ReplayWidget.cpp ReplayWidget.h)

target_link_libraries(interface
        PUBLIC Qt5::Core
        PUBLIC Qt5::Gui
        PUBLIC Qt5::Widgets
        PUBLIC proto
        PRIVATE math
        PRIVATE referee
        PUBLIC interfaceAPI
        PRIVATE protoUtils
        PRIVATE field
        PRIVATE logger
)
target_include_directories(interface
        PRIVATE include/interface
        INTERFACE include
        )