
cmake_minimum_required(VERSION 3.28.3)

project(aggregates_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(aggregates_samples_lib)

target_sources(aggregates_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/aggregates_construct.cxx
      src/aggregates_ctad.cxx
      src/aggregates_initialization.cxx
      src/aggredates_pack_expasion.cxx
      )

add_executable(aggregates_samples
  src/main.cpp)

target_link_libraries(aggregates_samples aggregates_samples_lib)