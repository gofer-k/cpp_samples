
cmake_minimum_required(VERSION 3.28.3)

project(UserUtilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(UserUtilities SHARED)

# Specify include directories
# target_include_directories(utilities_cpp PUBLIC src)

target_sources(UserUtilities
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/benchmarks.cxx
      src/print_tools.cxx    
      src/user_utilities.cxx 
      )

target_compile_features(UserUtilities PUBLIC cxx_std_23)

# Install the library
install(TARGETS UserUtilities
        EXPORT UserUtilitiesTargets 
        FILE_SET CXX_MODULES DESTINATION include/user_utilities
        LIBRARY DESTINATION build
        ARCHIVE DESTINATION build)

# Install the include directory
install(DIRECTORY src/user_utilities/
        DESTINATION include/user_utilities)

# Install the config files
install(EXPORT UserUtilitiesTargets
        FILE UtilitiesConfig.cmake
        NAMESPACE Utilities::
        DESTINATION lib/cmake/user_utilities)

# Install the config files
install(FILES UserUtilitiesConfig.cmake UserUtilitiesConfigVersion.cmake
        DESTINATION build/cmake/user_utilities)