cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(StarTL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMATCH_MOCKUP")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#include_directories()
link_directories(${PROJECT_SOURCE_DIR}/bin )


add_subdirectory(test)