#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "ops.hpp"

namespace optimus 
{
    
    namespace ops 
    {
        class TimeSeriesMad: public TimeSeriesOP
        {
            protected:
                double sum = 0.0;
                double mad = 0.0;
                double get() const override;
                void update(double x) override;
                
            public:
                TimeSeriesMad(size_t n): TimeSeriesOP(n) {};
                ~TimeSeriesMad() = default;
        };
    }

}

