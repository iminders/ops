#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

namespace optimus 
{
    namespace ops 
    {
        class TimeSeriesOP
        {
            protected:
                std::vector<double> buffer;  // 动态大小的存储窗口
                size_t write_pos = 0;        // 环形写入位置
                size_t count = 0;            // 当前元素计数
                size_t window_size;          // 窗口大小
                double sum = 0.0;            // 窗口总和

                virtual void update(double x) {};
                virtual double get() const = 0;
                
            public:
                TimeSeriesOP(size_t n) : window_size(n) {
                    buffer.resize(n, 0.0);
                }
                ~TimeSeriesOP() = default;
                
                // x: 不能为为 Nan, DBL_MAX, DBL_MIN, 需要在调用之前进行检查, 否则可能出现错误的结果Nan
                // 但程序不会崩溃
                double forward(double x) {
                    update(x);
                    return get();
                };
        };
    }

}

