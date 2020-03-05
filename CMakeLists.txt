cmake_minimum_required(VERSION 3.15)
project(icp_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(icp_project main.cpp StreetMap.cpp StreetMap.h Coordinates.cpp Coordinates.h Street.cpp Street.h Stop.cpp Stop.h)