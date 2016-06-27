cmake_minimum_required(VERSION 2.8.5)
project(navigation C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(SOURCE_FILES main.cpp include/navigator.h)

include_directories(include jsoncpp/dist)

add_executable(navigation ${SOURCE_FILES})

target_link_libraries(navigation curl)
#target_link_libraries(navigation curl wiringPi wiringPiDev) for led