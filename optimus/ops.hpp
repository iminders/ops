#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cfloat>

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

                virtual void update(double x) {};
                virtual double get() = 0;
                
            public:
                TimeSeriesOP(size_t n) : window_size(n) {
                    if (n <= 0) {
                        throw std::invalid_argument("window_size must be a positive integer");
                    }
                    buffer.resize(n, 0.0);
                }
                ~TimeSeriesOP() = default;
                
                // Nan, DBL_MAX, DBL_MIN, 这些值不会更新到buffer中，直接返回Nan
                // 但程序不会崩溃
                virtual double forward(double x) {
                    if (std::isnan(x) || std::isinf(x) || x==DBL_MAX || x==DBL_MIN) {
                        return NAN;
                    }
                    update(x);
                    return get();
                };
        };


        class TimeSeriesPairOP
        {
            protected:
                std::vector<double> xbuffer;  // 动态大小的存储窗口
                std::vector<double> ybuffer;  // 动态大小的存储窗口
                size_t write_pos = 0;        // 环形写入位置
                size_t count = 0;            // 当前元素计数
                size_t window_size;          // 窗口大小

                virtual void update(double x, double y) {};
                virtual double get() = 0;
                
            public:
                TimeSeriesPairOP(size_t n) : window_size(n) {
                    if (n <= 0) {
                        throw std::invalid_argument("window_size must be a positive integer");
                    }
                    xbuffer.resize(n, 0.0);
                    ybuffer.resize(n, 0.0);
                }
                ~TimeSeriesPairOP() = default;
                
                // Nan, DBL_MAX, DBL_MIN, 这些值不会更新到buffer中，直接返回Nan
                // 但程序不会崩溃
                virtual double forward(double x, double y) {
                    if (std::isnan(x) || std::isinf(x) || x==DBL_MAX || x==DBL_MIN || 
                        std::isnan(y) || std::isinf(y) || y==DBL_MAX || y==DBL_MIN) {
                        return NAN;
                    }
                    update(x, y);
                    return get();
                };
        };
    }

}

