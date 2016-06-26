cmake_minimum_required(VERSION 2.8.4)
project(tests C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(SOURCE_FILES NavigatorTests.h runner.cpp)
add_executable(tests ${SOURCE_FILES})

include_directories(include ../include)