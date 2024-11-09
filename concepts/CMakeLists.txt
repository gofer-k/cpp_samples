
cmake_minimum_required(VERSION 3.27.4)

project(ConceptsSamples)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ConceptsSamples src/main.cpp)

target_include_directories(ConceptsSamples
  PRIVATE
    include)
