cmake_minimum_required(VERSION 3.20)
project(Doodle)

set(CMAKE_CXX_STANDARD 14)

add_executable(Doodle WIN32 main.cpp doodle.cpp doodle.h platform.cpp platform.h)
