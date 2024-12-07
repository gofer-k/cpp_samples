
cmake_minimum_required(VERSION 3.28.3)

project(method_specifications_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(method_specifications_samples_lib)

target_sources(method_specifications_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/method_specifications.cxx     
      )

add_executable(method_specifications_samples
  src/main.cxx)

target_link_libraries(method_specifications_samples method_specifications_samples_lib)