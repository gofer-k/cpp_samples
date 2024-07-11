
cmake_minimum_required(VERSION 3.27.4)

project(CppSamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CppSamples ranges_snippets.cpp)

target_include_directories(CppSamples
    PRIVATE
        RangesFilters)

target_link_libraries(CppSamples
    RangesFilters)

add_subdirectory(ranges)