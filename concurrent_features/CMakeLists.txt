
cmake_minimum_required(VERSION 3.28.3)

project(concurrent_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Specify the path to the installed library
set(CMAKE_PREFIX_PATH "/home/useme/develoment/dev/cpp_samples/user_utilities")

find_package(UserUtilities REQUIRED)

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(concurrent_samples_lib)

# Enable external module interfaces source
add_subdirectory(${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR}/user_utilities_build)

target_sources(concurrent_samples_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/async_features.cxx
      src/barrier_samples.cxx
      src/free_lock_samples.cxx
      src/jthread_sample.cxx    
      src/latch_samples.cxx  
      src/locks_samples.cxx
      src/mutual_exclusion_samples.cxx
      src/route.cxx     
      )

target_link_libraries(concurrent_samples_lib UserUtilities)

add_executable(concurrent_samples
  src/main.cxx)

target_link_libraries(concurrent_samples concurrent_samples_lib UserUtilities)