
#include "ts_sum.hpp"

using namespace optimus::ops;


void TimeSeriesSum::update(double x)  {
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

