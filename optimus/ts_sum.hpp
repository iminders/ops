#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "ops.hpp"

namespace optimus 
{
    
    namespace ops 
    {
        class TimeSeriesSum: public TimeSeriesOP
        {
            private:
                void update(double x) override;
                double get() const override {return sum;};
                
            public:
                TimeSeriesSum(size_t n): TimeSeriesOP(n) {};
                ~TimeSeriesSum() = default;

                double forward(double x);
        };
    }

}

