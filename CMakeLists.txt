cmake_minimum_required(VERSION 3.15)
project(icp_project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

file (GLOB qt_app
        "src/View/*.cpp"
        "src/View/*.h"
        "src/Bus/*h"
        "src/Bus/*cpp"
        "src/*.cpp"
        "src/*.h"
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

add_executable(icp_project src/main.cpp src/main.h src/Bus/Bus.cpp src/Bus/Bus.h src/Bus/BusRouteMap.cpp src/Bus/BusRouteMap.h
        src/Bus/Garage.cpp src/Bus/Garage.h src/Bus/Stop.cpp src/Bus/Stop.h src/View/Coordinates.h
        src/View/MainWindow.cpp src/View/MainWindow.h src/View/Scene.cpp src/View/Scene.h src/View/Square.cpp src/View/Square.h
        src/View/Street.cpp src/View/Street.h src/View/StreetMap.cpp src/View/StreetMap.h src/View/Timer.cpp
        src/View/Timer.h src/Functions.cpp src/Functions.h)
#add_executable(icp_project ${qt_app} src/View/Timer.cpp src/View/Timer.h)
target_link_libraries(icp_project Qt5::Core Qt5::Widgets Qt5::Quick)