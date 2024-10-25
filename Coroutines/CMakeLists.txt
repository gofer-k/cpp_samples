
cmake_minimum_required(VERSION 3.27.4)

project(CoroutinesSamples)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CoroutinesSamples src/main.cpp)

target_include_directories(CoroutinesSamples
  PRIVATE
    include)
