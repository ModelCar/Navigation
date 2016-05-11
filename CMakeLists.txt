cmake_minimum_required(VERSION 2.8)
project(navigation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

add_executable(navigation ${SOURCE_FILES})

