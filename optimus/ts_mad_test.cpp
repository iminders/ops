#include <gtest/gtest.h>
#include "ts_mad.hpp"

using namespace std;
using namespace optimus::ops;


class TimeSeriesMadTest : public ::testing::Test {
    protected:
        void validate_node(TimeSeriesMad& node, const std::vector<double>& inputs, 
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


    // Positive test cases
    TEST_F(TimeSeriesMadTest, HandlesBasicWindowOperation) {
        TimeSeriesMad node(3);
        const std::vector<double> inputs{1,2,3,4,5,6};
        const std::vector<double> expected{0,0.5,2.0/3.0,2.0/3.0,2.0/3.0,2.0/3.0};
        validate_node(node, inputs, expected);
    }

    TEST_F(TimeSeriesMadTest, SingleElementWindow) {
        TimeSeriesMad node(1);
        const std::vector<double> inputs{5,7,3,-2};
        const std::vector<double> expected{0,0,0,0};
        validate_node(node, inputs, expected);
    }


    TEST_F(TimeSeriesMadTest, AllElementsSame) {
        TimeSeriesMad node(3);
        const std::vector<double> inputs{5.0,5.0,5.0,5.0,5.0};
        const std::vector<double> expected{0.0,0.0,0.0,0.0,0.0};
        validate_node(node, inputs, expected);
    }

    TEST_F(TimeSeriesMadTest, MaintainsPrecisionWithDecimals) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{0.1,0.2,0.3,0.4};
        const std::vector<double> expected{0.0,0.05,0.05,0.05};
        validate_node(node, inputs, expected);
    }

    // Negative test cases
    TEST_F(TimeSeriesMadTest, HandlesNegativeNumbers) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{-2,-1,-3,-4};
        const std::vector<double> expected{0.0,0.5,1.0,0.5};
        validate_node(node, inputs, expected);
    }

    TEST_F(TimeSeriesMadTest, HandlesMixedSignNumbers) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{-1.5, 3.0, -2.0};
        const std::vector<double> expected{0.0, 2.25, 2.5};
        validate_node(node, inputs, expected);
    }

    // Behavioral tests
    TEST_F(TimeSeriesMadTest, MaintainsRingBufferCorrectly) {
        TimeSeriesMad node(4);
        const std::vector<double> inputs{1,2,3,4,5,6,7};
        const std::vector<double> expected{0,0.5,2.0/3.0,1.0,1.0,1.0,1.0};
        validate_node(node, inputs, expected);
    }


    // Nan value tests
    TEST_F(TimeSeriesMadTest, HandlesNaNInputs) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{NAN,1.0,2.0,NAN,3.0};
        const std::vector<double> expected{NAN,0.0, 0.5,NAN,0.5};
        validate_node(node, inputs, expected);
    }

    // Infinity value tests
    TEST_F(TimeSeriesMadTest, HandlesInfinity) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{INFINITY,1.0,2.0,INFINITY,3.0};
        const std::vector<double> expected{NAN,0.0, 0.5,NAN,0.5};
        validate_node(node, inputs, expected);
    }

    TEST_F(TimeSeriesMadTest, HandlesMaximumPrecision) {
        TimeSeriesMad node(2);
        const std::vector<double> inputs{DBL_MAX,1.0,2.0,DBL_MIN,3.0};
        const std::vector<double> expected{NAN,0.0, 0.5,NAN,0.5};
        validate_node(node, inputs, expected);
    }



int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}