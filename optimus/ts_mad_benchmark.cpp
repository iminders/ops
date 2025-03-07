#include <benchmark/benchmark.h>
#include <random>
#include "ts_mad.hpp"

using namespace optimus::ops;

// 基础性能测试
static void BM_TimeSeriesMad(benchmark::State& state) {
    const size_t window_size = state.range(0);
    TimeSeriesMad node(window_size);
    
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
BENCHMARK(BM_TimeSeriesMad)
    ->Args({100})    // 窗口大小100
    ->Args({300})    // 窗口大小300
    ->Args({1000})   // 窗口大小1000
    ->Threads(1)    // 单线程测试
    ->ThreadPerCpu() // 增加多线程测试
    ->UseRealTime();

// 完全填充窗口后的稳定状态测试
static void BM_SteadyState(benchmark::State& state) {
    const size_t window_size = state.range(0);
    TimeSeriesMad node(window_size);
    
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
Running benchmark: ts_mad_benchmark
2025-03-07T09:08:05+00:00
Running /home/runner/work/ops/ops/build/ts_mad_benchmark
Run on (4 X 3240.96 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 512 KiB (x2)
  L3 Unified 32768 KiB (x1)
Load Average: 2.54, 0.80, 0.29
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_TimeSeriesMad/100/real_time/threads:1        30.2 ns         30.2 ns     23203835 items_per_second=33.1342M/s
BM_TimeSeriesMad/100/real_time/threads:4        39.7 ns         39.6 ns     17683576 items_per_second=25.2025M/s
BM_TimeSeriesMad/300/real_time/threads:1        70.3 ns         70.3 ns      9944743 items_per_second=14.2159M/s
BM_TimeSeriesMad/300/real_time/threads:4         108 ns          107 ns      6510980 items_per_second=9.2857M/s
BM_TimeSeriesMad/1000/real_time/threads:1        230 ns          230 ns      3037954 items_per_second=4.35483M/s
BM_TimeSeriesMad/1000/real_time/threads:4        326 ns          325 ns      2149040 items_per_second=3.06369M/s
BM_SteadyState/1000                              222 ns          222 ns      3153022 items_per_second=4.50631M/s
*/