
#include "ts_mad.hpp"

using namespace optimus::ops;


void TimeSeriesMad::update(double x) {
    if (count < window_size) {
        sum += x;
        buffer[write_pos] = x;
        count++;
    } else {
        sum += x - buffer[write_pos];
        buffer[write_pos] = x;
    }
    write_pos = (write_pos + 1) % window_size;

}

double TimeSeriesMad::get() const {
    double mean = sum / count;
    double sum_abs = 0.0;
    for (int i = 0; i < count; ++i) {
        sum_abs += std::fabs(buffer[i] - mean);
    }
    return sum_abs / count;
} 
