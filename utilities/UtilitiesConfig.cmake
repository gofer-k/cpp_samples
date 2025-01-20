# Include the version file
include(CMakeFindDependencyMacro)

set(UTILITIES 1.0.0)

# Specify the include directories
set(UTILITIES_INCLUDES_DIRS "${CMAKE_CURRENT_LIST_DIR}/src")

set(UTILITIES_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Specify the library files
set(UTILITIES_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/build/libUtilities.a")

# Mark the variables to be used in the find_package
set(UTILITIES_FOUND TRUE)

# Provide the include directories and libraries
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Utilities DEFAULT_MSG UTILITIES_FOUND)

# Provide the targets for the library
if(UTiLITIES_CPP_FOUND)
    mark_as_advanced(FORCE UTILITIES_INCLUDES_DIRS UTILITIES_LIBRARIES)
    set(UTILITIES_INCLUDES_DIRS "${UTILITIES_INCLUDES_DIRS}" CACHE PATH "Include directory for Utilities")
    set(UTILITIES_LIBRARIES "${UTILITIES_LIBRARIES}" CACHE FILEPATH "Library file for Utilities")
endif()