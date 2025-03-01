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
*/