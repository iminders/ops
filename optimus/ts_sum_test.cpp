#include <gtest/gtest.h>
#include "ts_sum.hpp"

using namespace optimus::ops;

TEST(TimeSeriesSumTest, MethodRenaming) {
    TimeSeriesSum node(3);
    
    EXPECT_DOUBLE_EQ(node.forward(1), 1.0);
    
    EXPECT_DOUBLE_EQ(node.forward(2), 3.0);
}

TEST(TimeSeriesSumTest, ForwardMethod) {
    TimeSeriesSum node(3);
    // 测试forward方法
    EXPECT_DOUBLE_EQ(node.forward(1), 1.0);  // [1]
    EXPECT_DOUBLE_EQ(node.forward(2), 3.0);  // [1,2]
    EXPECT_DOUBLE_EQ(node.forward(3), 6.0);  // [1,2,3]
    EXPECT_DOUBLE_EQ(node.forward(4), 9.0);  // [2,3,4]
    
}

TEST(TimeSeriesSumTest, EdgeCaseForward) {
    // 测试空窗口的forward
    TimeSeriesSum zero_window(0);
    EXPECT_DOUBLE_EQ(zero_window.forward(5.0), 0.0);
    
    // 测试单元素窗口
    TimeSeriesSum single_window(1);
    EXPECT_DOUBLE_EQ(single_window.forward(2.5), 2.5);
    EXPECT_DOUBLE_EQ(single_window.forward(3.0), 3.0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}