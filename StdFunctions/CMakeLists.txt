
cmake_minimum_required(VERSION 3.27.4)

project(StdFunctions)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(StdFunctions src/main.cpp)

target_include_directories(StdFunctions
  PRIVATE
    include)
