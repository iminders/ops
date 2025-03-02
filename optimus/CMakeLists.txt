
# Main library configuration
# Creates a library from source files
add_library(ts_sum_lib
  ts_sum.hpp
  ts_sum.cpp
)

# Specifies include directories for the library
target_include_directories(ts_sum_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test executable configuration
# Creates an executable for testing
add_executable(test_ts_sum ts_sum_test.cpp)

# Links the test executable with the library and GoogleTest
target_link_libraries(test_ts_sum PRIVATE ts_sum_lib GTest::gtest_main)

# Enables testing and adds test target
enable_testing()
add_test(NAME test_ts_sum COMMAND test_ts_sum)

add_executable(benchmark_ts_sum ts_sum_benchmark.cpp)

# Links the benchmark executable with the library and Google Benchmark
target_link_libraries(benchmark_ts_sum
  PRIVATE
  ts_sum_lib
  benchmark::benchmark
)
