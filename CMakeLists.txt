cmake_minimum_required(VERSION 3.17)
project(WallCSS)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(WallCSS main.cpp wallhack.h wallhack.cpp)