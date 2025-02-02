
cmake_minimum_required(VERSION 3.28.3)

project(language_features LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(language_features_lib)

target_sources(language_features_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/deducing_this.cxx
      src/callable_features.cxx
      src/forwarding_types.cxx)


add_executable(language_features
  src/main.cxx)

target_link_libraries(language_features language_features_lib)