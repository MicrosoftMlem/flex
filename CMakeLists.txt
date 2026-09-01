cmake_minimum_required(VERSION 3.16)
project(flex)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(flex src/main.cpp)
