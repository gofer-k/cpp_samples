
cmake_minimum_required(VERSION 3.27.4)

project(ranges_samples)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ranges_samples 
  src/ranges_snippets.cpp)

target_include_directories(ranges_samples
  PRIVATE
    src/include)
