
cmake_minimum_required(VERSION 3.28.3)

project(move_semantic_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(move_semantic_samples
  src/main.cpp)
