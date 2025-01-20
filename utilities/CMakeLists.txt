
cmake_minimum_required(VERSION 3.28.3)

project(Utilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)

# This necessarily with GCC 14 onward
set(CMAKE_CXX_FLAGS "-fmodules-ts")

# Turning off extensions avoids an issue with the clang 16 compiler
# clang 17 and greater can avoid this setting
#set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Utilities SHARED)

# Specify include directories
# target_include_directories(utilities_cpp PUBLIC src)

target_sources(Utilities
  PUBLIC
    FILE_SET CXX_MODULES FILES
    # implemation needs is suffic 'cxx', 'cpp'
      src/print_collection.cxx     
      )

target_compile_features(Utilities PUBLIC cxx_std_23)

# Install the library
install(TARGETS Utilities
        EXPORT UtilitiesTargets 
        FILE_SET CXX_MODULES DESTINATION include/Utilities
        LIBRARY DESTINATION build
        ARCHIVE DESTINATION build)

# Install the include directory
install(DIRECTORY src/Utilities/
        DESTINATION include/Utilities)

# Install the config files
install(EXPORT UtilitiesTargets
        FILE UtilitiesConfig.cmake
        NAMESPACE Utilities::
        DESTINATION lib/cmake/Utilities)

# Install the config files
install(FILES UtilitiesConfig.cmake UtilitiesConfigVersion.cmake
        DESTINATION build/cmake/Utilities)