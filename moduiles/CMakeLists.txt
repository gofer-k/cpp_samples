
cmake_minimum_required(VERSION 3.28.3)

project(ModulesSamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(root_module)

target_sources(root_module
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/root_module.cxx)


add_executable(ModulesSamples
  src/main.cxx)

target_link_libraries(ModulesSamples root_module)