cmake_minimum_required(VERSION 3.15)
project(icp_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "/home/alexandra/Qt/5.14.1/gcc_64/lib/cmake")

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

#add_executable(icp_project main.cpp src/StreetMap.cpp src/StreetMap.h src/Coordinates.cpp src/Coordinates.h src/Street.cpp src/Street.h src/Stop.cpp src/Stop.h main.h src/Functions.cpp src/Functions.h)
add_executable(icp_project ${qt_app} src/View/Timer.cpp src/View/Timer.h)
target_link_libraries(icp_project Qt5::Core Qt5::Widgets Qt5::Quick)