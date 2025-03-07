#include <benchmark/benchmark.h>
#include <random>
#include "ts_sum.hpp"

using namespace optimus::ops;

// 基础性能测试
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
Running benchmark: ts_sum_benchmark
2025-03-07T07:21:07+00:00
Running /home/runner/work/ops/ops/build/ts_sum_benchmark
Run on (4 X 3241.99 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 512 KiB (x2)
  L3 Unified 32768 KiB (x1)
Load Average: 2.11, 0.87, 0.34
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_TimeSeriesSum/100/real_time/threads:1        5.30 ns         5.30 ns    131863398 items_per_second=188.537M/s
BM_TimeSeriesSum/100/real_time/threads:4        5.38 ns         5.37 ns    130416980 items_per_second=185.902M/s
BM_TimeSeriesSum/300/real_time/threads:1        5.30 ns         5.30 ns    132061167 items_per_second=188.562M/s
BM_TimeSeriesSum/300/real_time/threads:4        5.38 ns         5.37 ns    130206772 items_per_second=185.747M/s
BM_TimeSeriesSum/1000/real_time/threads:1       5.30 ns         5.30 ns    131897577 items_per_second=188.558M/s
BM_TimeSeriesSum/1000/real_time/threads:4       5.42 ns         5.38 ns    130077724 items_per_second=184.51M/s
BM_SteadyState/1000                             4.82 ns         4.82 ns    145331401 items_per_second=207.629M/s
*/