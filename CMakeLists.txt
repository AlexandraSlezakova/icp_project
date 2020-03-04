cmake_minimum_required(VERSION 3.15)
project(icp_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(icp_project main.cpp StreetMap.cpp StreetMap.h Coordinates.cpp Coordinates.h)