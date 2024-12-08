
cmake_minimum_required(VERSION 3.27.4)

project(std_algo_caveats)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(std_algo_caveats
  src/main.cpp)

target_include_directories(std_algo_caveats
  PRIVATE
    include)
