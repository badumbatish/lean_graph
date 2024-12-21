cmake_minimum_required(VERSION 3.20)

project(LeanGraph)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Catch2 3 REQUIRED)

add_subdirectory(core)
