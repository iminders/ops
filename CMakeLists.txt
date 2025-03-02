cmake_minimum_required(VERSION 3.14)
project(ops)

set(CMAKE_CXX_STANDARD 14)

# GoogleTest 配置
find_package(GTest CONFIG QUIET)  # 优先查找 CMake 配置模式
if (TARGET GTest::gtest AND TARGET GTest::gtest_main)
    message(STATUS "Found system GoogleTest")
else()
    message(STATUS "GoogleTest not found, fetching from source...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Google Benchmark 配置
find_package(benchmark CONFIG QUIET)  # 优先查找 CMake 配置模式
if (TARGET benchmark::benchmark AND TARGET benchmark::benchmark_main)
    message(STATUS "Found system Google Benchmark")
else()
    message(STATUS "Google Benchmark not found, fetching from source...")
    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
    )
    FetchContent_MakeAvailable(benchmark)
endif()

# Main library
add_library(ts_sum_lib
  optimus/ts_sum.hpp
  optimus/ts_sum.cpp
)

target_include_directories(ts_sum_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/optimus
)

# Test executable
add_executable(test_ts_sum optimus/ts_sum_test.cpp)

target_link_libraries(test_ts_sum PRIVATE ts_sum_lib GTest::gtest_main)

enable_testing()
add_test(NAME test_ts_sum COMMAND test_ts_sum)

# Benchmark executable
add_executable(benchmark_ts_sum
  optimus/ts_sum_benchmark.cpp
)

target_link_libraries(benchmark_ts_sum
  PRIVATE
  ts_sum_lib
  benchmark::benchmark
)
