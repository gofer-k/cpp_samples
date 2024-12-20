
cmake_minimum_required(VERSION 3.28.3)

project(smartptr_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(smartptr_samples_lib)

target_sources(smartptr_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/inout_ptr.cxx)

add_executable(smartptr_samples
  src/main.cxx)

target_link_libraries(smartptr_samples smartptr_samples_lib)