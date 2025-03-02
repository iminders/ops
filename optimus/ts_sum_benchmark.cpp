#include <benchmark/benchmark.h>
#include <random>
#include "ts_sum.hpp"

using namespace optimus::ops;

static void BM_TimeSeriesSum(benchmark::State& state) {
    const size_t window_size = state.range(0);
    TimeSeriesSum node(window_size);
    
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    const size_t data_size = 100'000;
    std::vector<double> test_data(data_size);
    std::generate(test_data.begin(), test_data.end(), [&]{ return dist(gen); });
    
    size_t data_index = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(node.forward(test_data[data_index]));
        data_index = (data_index + 1) % data_size;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_TimeSeriesSum)
    ->Args({100})    // 窗口大小100
    ->Args({300})    // 窗口大小300
    ->Args({1000})   // 窗口大小1000
    ->Threads(1)    // 单线程测试
    ->ThreadPerCpu() // 增加多线程测试
    ->UseRealTime();

// 空窗口极端情况测试
static void BM_EmptyWindow(benchmark::State& state) {
    TimeSeriesSum node(0); // 无效窗口
    for (auto _ : state) {
        benchmark::DoNotOptimize(node.forward(1.0));
    }
}
BENCHMARK(BM_EmptyWindow);

// 完全填充窗口后的稳定状态测试
static void BM_SteadyState(benchmark::State& state) {
    const size_t window_size = state.range(0);
    TimeSeriesSum node(window_size);
    
    // 预填充窗口
    for (size_t i = 0; i < window_size; ++i) {
        node.forward(1.0);
    }
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(node.forward(2.0));
    }
    
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SteadyState)->Arg(1000);

BENCHMARK_MAIN();

/*
Mac M1上不开启编译化
g++ -std=c++14 -Wall -pthread  -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark && ./ts_sum_benchmark

Run on (8 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x8)
Load Average: 2.09, 1.90, 2.14
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_TimeSeriesSum/100/real_time/threads:1        12.9 ns         12.9 ns     54231300 items_per_second=77.4243M/s
BM_TimeSeriesSum/100/real_time/threads:8        16.1 ns         14.8 ns     42399616 items_per_second=61.9296M/s
BM_TimeSeriesSum/300/real_time/threads:1        12.9 ns         12.9 ns     54160092 items_per_second=77.2456M/s
BM_TimeSeriesSum/300/real_time/threads:8        16.7 ns         14.2 ns     44999976 items_per_second=59.9973M/s
BM_TimeSeriesSum/1000/real_time/threads:1       12.9 ns         12.9 ns     54140264 items_per_second=77.3639M/s
BM_TimeSeriesSum/1000/real_time/threads:8       16.4 ns         13.2 ns     42968264 items_per_second=60.8748M/s
BM_EmptyWindow                                  6.17 ns         6.15 ns    113941564
BM_SteadyState/1000                             11.0 ns         11.0 ns     63503583 items_per_second=90.8384M/s

Mac M1上开启编译化:
g++ -std=c++14 -Wall -pthread  -O3 -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark && ./ts_sum_benchmark

Run on (8 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x8)
Load Average: 2.72, 2.61, 2.31
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_TimeSeriesSum/100/real_time/threads:1        6.83 ns         6.82 ns    102060777 items_per_second=146.465M/s
BM_TimeSeriesSum/100/real_time/threads:8        7.40 ns         7.10 ns     92945160 items_per_second=135.144M/s
BM_TimeSeriesSum/300/real_time/threads:1        6.81 ns         6.81 ns    102326278 items_per_second=146.783M/s
BM_TimeSeriesSum/300/real_time/threads:8        7.46 ns         7.09 ns     92706664 items_per_second=134.12M/s
BM_TimeSeriesSum/1000/real_time/threads:1       6.82 ns         6.81 ns    102420227 items_per_second=146.669M/s
BM_TimeSeriesSum/1000/real_time/threads:8       7.43 ns         7.11 ns     88670792 items_per_second=134.569M/s
BM_EmptyWindow                                  2.47 ns         2.46 ns    284460338
BM_SteadyState/1000                             6.82 ns         6.81 ns    102336189 items_per_second=146.793M/s

Mac M1上开启编译化:
g++ -std=c++14 -Wall -pthread  -mcpu=apple-m1 -O3 -ffast-math -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark && ./ts_sum_benchmark

Run on (8 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x8)
Load Average: 2.20, 2.47, 2.27
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_TimeSeriesSum/100/real_time/threads:1        6.81 ns         6.81 ns    101976834 items_per_second=146.752M/s
BM_TimeSeriesSum/100/real_time/threads:8        7.72 ns         7.41 ns     96899992 items_per_second=129.45M/s
BM_TimeSeriesSum/300/real_time/threads:1        6.82 ns         6.82 ns    102276692 items_per_second=146.572M/s
BM_TimeSeriesSum/300/real_time/threads:8        7.53 ns         7.12 ns     91394560 items_per_second=132.783M/s
BM_TimeSeriesSum/1000/real_time/threads:1       6.82 ns         6.81 ns    102261002 items_per_second=146.603M/s
BM_TimeSeriesSum/1000/real_time/threads:8       7.88 ns         7.10 ns     93632176 items_per_second=126.897M/s
BM_EmptyWindow                                  2.48 ns         2.47 ns    284388686
BM_SteadyState/1000                             6.82 ns         6.81 ns    102511533 items_per_second=146.769M/s
*/