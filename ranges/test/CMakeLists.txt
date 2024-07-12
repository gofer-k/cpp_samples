project(RangesFiltersTest)

find_package(GTest REQUIRED)

# Add your test files
set(SOURCES
    filters_test.cpp
)

# Add the executable
add_executable(RangesFiltersTest ${SOURCES})

# Link any libraries needed for your project
target_link_libraries(RangesFiltersTest 
    ${GTEST_LIBRARY_DEBUG}
    ${GTEST_MAIN_LIBRARY_DEBUG}
    RangesFilters)

# Include any directories needed for your project
target_include_directories(RangesFiltersTest 
    PRIVATE
        ${GTEST_INCLUDE_DIR}
    INTERFACE
        ../src)

# enable testing functionality
enable_testing()

add_test(NAME FilterRangeTest COMMAND RangesFiltersTest)

