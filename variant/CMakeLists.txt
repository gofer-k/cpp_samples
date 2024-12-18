
cmake_minimum_required(VERSION 3.28.3)

project(variant_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)
add_library(variant_samples_lib)

target_sources(variant_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
    src/create_variant.cxx
    src/visit_variant.cxx)

add_executable(variant_samples
  src/main.cxx)

target_link_libraries(variant_samples variant_samples_lib)
