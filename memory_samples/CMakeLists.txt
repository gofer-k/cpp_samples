
cmake_minimum_required(VERSION 3.28.3)

project(MemorySamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")
# Specify the path to the installed library
set(CMAKE_PREFIX_PATH "../user_utilities")

find_package(UserUtilities REQUIRED)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

# Enable external module interfaces source
add_subdirectory(${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR}/user_utilities_build)

add_library(MemorySamples_lib)

# Specify include directories
# target_include_directories(utilities_cpp PUBLIC src)

target_sources(MemorySamples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/allocation_samples.cxx
      )

target_link_libraries(MemorySamples_lib 
  PRIVATE
    UserUtilities)

add_executable(MemorySamples
  src/main.cxx)

target_link_libraries(MemorySamples MemorySamples_lib)