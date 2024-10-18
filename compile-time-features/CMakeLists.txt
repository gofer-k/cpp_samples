
cmake_minimum_required(VERSION 3.27.4)

project(CompileTileFeatures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(CompileTileFeatures src/main.cpp)

target_include_directories(CompileTileFeatures
  PRIVATE
    include)
