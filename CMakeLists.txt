# Minimum required CMake version for this project
cmake_minimum_required(VERSION 3.14)

# Project name and basic configuration
project(ops)

# Set C++ standard to C++14
set(CMAKE_CXX_STANDARD 14)

# Google Test configuration 
# Tries to find system-wide GoogleTest first
find_package(GTest CONFIG QUIET)

if (TARGET GTest::gtest AND TARGET GTest::gtest_main)
    message(STATUS "Found system GoogleTest")
else()
    message(STATUS "GoogleTest not found, fetching from source...")
    # Fetch GoogleTest from repository if not found
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Google Benchmark configuration
# Tries to find system-wide Google Benchmark first
find_package(benchmark CONFIG QUIET)

if (TARGET benchmark::benchmark AND TARGET benchmark::benchmark_main)
    message(STATUS "Found system Google Benchmark")
else()
    message(STATUS "Google Benchmark not found, fetching from source...")
    # Fetch Google Benchmark from repository if not found
    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
    )
    FetchContent_MakeAvailable(benchmark)
endif()


add_subdirectory(optimus)