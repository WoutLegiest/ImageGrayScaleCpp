cmake_minimum_required(VERSION 3.12)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp lodepng.cpp)

add_executable(HelloWorld ${SOURCE_FILES})