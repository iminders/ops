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

*/