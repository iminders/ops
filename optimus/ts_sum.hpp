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
            protected:
                double sum = 0.0;
                void update(double x) override;
                double get() const override {return sum;};
                
            public:
                TimeSeriesSum(int n): TimeSeriesOP(n) {};
                ~TimeSeriesSum() = default;
        };
    }

}

