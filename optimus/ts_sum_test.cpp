#include <gtest/gtest.h>
#include "ts_sum.hpp"

using namespace optimus::ops;

class TimeSeriesSumTest : public ::testing::Test {
    protected:
        void validate_node(TimeSeriesSum& node, const std::vector<double>& inputs, 
                          const std::vector<double>& expected) {
            for(size_t i = 0; i < inputs.size(); ++i) {
                const auto result = node.forward(inputs[i]);
                if(std::isnan(expected[i])) {
                    ASSERT_TRUE(std::isnan(result)) << "At index: " << i;
                } else {
                    ASSERT_NEAR(result, expected[i], 1e-9) << "At index: " << i;
                }
            }
        }
    };
    
    // 测试用例：非法窗口
    TEST_F(TimeSeriesSumTest, InvalidWindowSize) {
        EXPECT_THROW(TimeSeriesSum node(0), std::invalid_argument);
        EXPECT_THROW(TimeSeriesSum node(-1), std::length_error);
    
    }
    
    // Positive test cases
    TEST_F(TimeSeriesSumTest, HandlesBasicWindowOperation) {
        TimeSeriesSum node(3);
        const std::vector<double> inputs{1,2,3,4,5,6};
        const std::vector<double> expected{1,3,6,9,12,15};
        validate_node(node, inputs, expected);
    }
    
    TEST_F(TimeSeriesSumTest, SingleElementWindow) {
        TimeSeriesSum node(1);
        const std::vector<double> inputs{5,7,3,-2};
        const std::vector<double> expected{5,7,3,-2};
        validate_node(node, inputs, expected);
    }
    
    TEST_F(TimeSeriesSumTest, MaintainsPrecisionWithDecimals) {
        TimeSeriesSum node(2);
        const std::vector<double> inputs{0.1,0.2,0.3,0.4};
        const std::vector<double> expected{0.1,0.3,0.5,0.7};
        validate_node(node, inputs, expected);
    }
    
    // Negative test cases
    TEST_F(TimeSeriesSumTest, HandlesNegativeNumbers) {
        TimeSeriesSum node(3);
        const std::vector<double> inputs{-1,-2,-3,-4};
        const std::vector<double> expected{-1,-3,-6,-9};
        validate_node(node, inputs, expected);
    }
    
    TEST_F(TimeSeriesSumTest, HandlesMixedSignNumbers) {
        TimeSeriesSum node(2);
        const std::vector<double> inputs{-1.5, 3.0, -2.0};
        const std::vector<double> expected{-1.5, 1.5, 1.0};
        validate_node(node, inputs, expected);
    }
    
    // Behavioral tests
    TEST_F(TimeSeriesSumTest, MaintainsRingBufferCorrectly) {
        TimeSeriesSum node(3);
        const std::vector<double> inputs{1,2,3,4,5,6,7};
        const std::vector<double> expected{1,3.0,6,9,12,15,18};
        validate_node(node, inputs, expected);
    }
    
    TEST_F(TimeSeriesSumTest, HandlesContinuousUpdates) {
        TimeSeriesSum node(1000);
        for(int i = 1; i <= 2000; ++i) {
            const double expected = i < 1000 ? (i*(i+1))/2.0 : (i + (i-999)) * 500.0;
            ASSERT_NEAR(node.forward(i), expected, 1e-6);
        }
    }
    
    // Nan value tests
    TEST_F(TimeSeriesSumTest, HandlesNaNInputs) {
        TimeSeriesSum node(2);
        const std::vector<double> inputs{NAN,1.0,2.0,NAN,3.0};
        const std::vector<double> expected{NAN,1.0,3.0,NAN,5.0};
        validate_node(node, inputs, expected);
    }
    
    // Infinity value tests
    TEST_F(TimeSeriesSumTest, HandlesInfinity) {
        TimeSeriesSum node(2);
        const std::vector<double> inputs{INFINITY,1.0,2.0,INFINITY,3.0};
        const std::vector<double> expected{NAN,1.0,3.0,NAN,5.0};
        validate_node(node, inputs, expected);
    }
    
    TEST_F(TimeSeriesSumTest, HandlesMaximumPrecision) {
        TimeSeriesSum node(2);
        const std::vector<double> inputs{DBL_MAX,1.0,2.0,DBL_MIN,3.0};
        const std::vector<double> expected{NAN,1.0,3.0,NAN,5.0};
        validate_node(node, inputs, expected);
    }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}