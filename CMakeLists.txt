cmake_minimum_required(VERSION 3.15)
project(icp_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(icp_project main.cpp src/StreetMap.cpp src/StreetMap.h src/Coordinates.cpp src/Coordinates.h src/Street.cpp src/Street.h src/Stop.cpp src/Stop.h main.h src/Functions.cpp src/Functions.h)