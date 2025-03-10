#pragma once

#include "ops.hpp"

namespace optimus 
{
    
    namespace ops 
    {   
        /* 
            Rolling Mean Absolute Deviation
            算法目前需要O(n)的复杂度，相对较慢
        */
        class TimeSeriesMad: public TimeSeriesOP
        {
            protected:
                double sum = 0.0;
                double mad = 0.0;
                double get() override;
                void update(double x) override;
                
            public:
                TimeSeriesMad(size_t n): TimeSeriesOP(n) {};
                ~TimeSeriesMad() = default;
        };
    }

}

