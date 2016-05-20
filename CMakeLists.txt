cmake_minimum_required(VERSION 3.5)
project(polltest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp EventLoop.cpp EventLoop.h Event.cpp Event.h EventHandler.cpp EventHandler.h EpollEvent.cpp EpollEvent.h Logger.cpp Logger.h)
add_executable(polltest ${SOURCE_FILES})