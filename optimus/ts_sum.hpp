#pragma once

#include "ops.hpp"

namespace optimus 
{
    namespace ops 
    {
        class TimeSeriesSum: public TimeSeriesOP
        {
            protected:
                double sum = 0.0;
                void update(double x) override;
                double get() override {return sum;};
                
            public:
                TimeSeriesSum(int n): TimeSeriesOP(n) {};
                ~TimeSeriesSum() = default;
        };
    }

}

