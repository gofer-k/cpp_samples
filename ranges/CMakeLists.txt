
project(RangesFilters)

# Add all the header files in your project directory
file(GLOB HEADER_FILES "*.hpp")

# Create a library target
add_library(RangesFilters INTERFACE)

# Include the header files for the library
target_sources(RangesFilters INTERFACE ${HEADER_FILES})

target_include_directories(RangesFilters INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
