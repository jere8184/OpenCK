cmake_minimum_required(VERSION 3.31)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 26)

project(OpenCK)

add_executable(OpenCK Main.cpp)

target_include_directories(OpenCK PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/parser)
add_subdirectory(${CMAKE_SOURCE_DIR}/scripting)
add_subdirectory(${CMAKE_SOURCE_DIR}/simulator)
