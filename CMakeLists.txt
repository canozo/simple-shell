cmake_minimum_required(VERSION 3.1)

project(
  shell
  VERSION 1.0
  DESCRIPTION "Shell simple"
  LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

add_executable(
  shell
  src/main.cpp
)
