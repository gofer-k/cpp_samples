
cmake_minimum_required(VERSION 3.28.3)

project(lambdes_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lambdas_samples_lib)

target_sources(lambdas_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/capture_this.cxx
      src/generic_lambda.cxx)

add_executable(lambdas_samples
  src/main.cpp)

target_link_libraries(lambdas_samples lambdas_samples_lib)