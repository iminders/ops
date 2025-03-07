
enable_testing()

add_library(ts_sum_lib ts_sum.hpp ts_sum.cpp)
add_executable(test_ts_sum ts_sum_test.cpp)
target_link_libraries(test_ts_sum PRIVATE ts_sum_lib GTest::gtest_main)
add_test(NAME test_ts_sum COMMAND test_ts_sum)
