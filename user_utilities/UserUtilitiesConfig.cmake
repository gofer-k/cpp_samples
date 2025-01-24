# Include the version file
include(CMakeFindDependencyMacro)

set(USER_UTILITIES 1.0.0)

# Specify the include directories
set(USER_UTILITIES_INCLUDES_DIRS "${CMAKE_CURRENT_LIST_DIR}/src")

set(USER_UTILITIES_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Specify the library files
set(USER_UTILITIES_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/build/libUserUtilities.a")

# Mark the variables to be used in the find_package
set(USER_UTILITIES_FOUND TRUE)

# Provide the include directories and libraries
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UserUtilities DEFAULT_MSG USER_UTILITIES_FOUND)

# Provide the targets for the library
if(USER_TiLITIES_CPP_FOUND)
    mark_as_advanced(FORCE USER_UTILITIES_INCLUDES_DIRS USER_UTILITIES_LIBRARIES)
    set(USER_UTILITIES_INCLUDES_DIRS "${USER_UTILITIES_INCLUDES_DIRS}" CACHE PATH "Include directory for User utilities")
    set(USER_UTILITIES_LIBRARIES "${USER_UTILITIES_LIBRARIES}" CACHE FILEPATH "Library file for User utilities")
endif()