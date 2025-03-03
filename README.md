# ops
[![UnitTest](https://github.com/iminders/ops/actions/workflows/unittest.yml/badge.svg)](https://github.com/iminders/ops/actions/workflows/unittest.yml)


## Quick Start

### Dependences Install

#### [googletest v1.16.0](https://github.com/google/googletest/blob/main/googletest/README.md)
```
git clone git@github.com:google/googletest.git -b v1.16.0
cd googletest        # Main directory of the cloned repository.
mkdir build          # Create a directory to hold the build output.
cd build
cmake ..             # Generate native build scripts for GoogleTest.goo

make
sudo make install    # Install in /usr/local/ by default
```

#### [google benchmark v1.9.1](https://github.com/google/benchmark?tab=readme-ov-file#installation)
```
# Check out the library.
git clone git@github.com:google/benchmark.git
# Go to the library root directory
cd benchmark
git checkout v1.9.1
# Make a build directory to place the build output.
cmake -E make_directory "build"
# Generate build system files with cmake, and download any dependencies.
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
# or, starting with CMake 3.13, use a simpler form:
# cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -S . -B "build"
# Build the library.
cmake --build "build" --config Release

cmake -E chdir "build" ctest --build-config Release

sudo cmake --build "build" --config Release --target install

```

### Run test && bench by cmake
```
git clone https://github.com/iminders/ops.git
cd ops

# unit test
make test

# benchmark
make bench
```

### Run test && bench by make
```
git clone https://github.com/iminders/ops.git
cd ops/optimus

# test ts_sum
make ts_sum_test

# benchmark ts_sum
make ts_sum_bench
```

### Compare benchmark results
[compare tool](https://github.com/google/benchmark/blob/main/docs/tools.md)
```
python tools/compare.py benchmarks <benchmark_baseline> <benchmark_contender> [benchmark options]...
```
Where <benchmark_baseline> and <benchmark_contender> either specify a benchmark executable file, or a JSON output file
The type of the input file is automatically detected. If a benchmark executable is specified then the benchmark is run to obtain the results. 
Otherwise the results are simply loaded from the output file.

Example for ts_sum -O3 opt flags 
```
# not_opt_ts_sum.json
make test && ./build/optimus/benchmark_ts_sum --benchmark_format=json > tools/testdata/old_ts_sum.json
# opt_ts_sum.json
make bench && ./build/optimus/benchmark_ts_sum --benchmark_format=json > tools/testdata/new_ts_sum.json

python tools/compare.py benchmarks tools/testdata/old_ts_sum.json tools/testdata/new_ts_sum.json
```

output:
```
Benchmark                                                   Time             CPU      Time Old      Time New       CPU Old       CPU New
----------------------------------------------------------------------------------------------------------------------------------------
BM_TimeSeriesSum/100/real_time/threads:1                 -0.7148         -0.7127            30             8            29             8
BM_TimeSeriesSum/100/real_time/threads:8                 -0.7589         -0.7449            48            11            43            11
BM_TimeSeriesSum/300/real_time/threads:1                 -0.7133         -0.7138            29             8            28             8
BM_TimeSeriesSum/300/real_time/threads:8                 -0.7278         -0.7433            45            12            43            11
BM_TimeSeriesSum/1000/real_time/threads:1                -0.6952         -0.6961            28             9            28             9
BM_TimeSeriesSum/1000/real_time/threads:8                -0.7224         -0.7460            46            13            44            11
BM_EmptyWindow                                           -0.7644         -0.7643             9             2             9             2
BM_SteadyState/1000                                      -0.5991         -0.6028            22             9            22             9
OVERALL_GEOMEAN                                          -0.7156         -0.7190             0             0             0             0
```


## TODO
  - [ ] Single input
    - [x] ts_sum
    - [ ] ts_mean
    - [ ] ts_var
    - [ ] ts_std
    - [ ] ts_max
    - [ ] ts_min
    - [ ] ts_ref
    - [ ] ts_skew
    - [ ] ts_kurt
    - [ ] ts_med
    - [ ] ts_mad
    - [ ] ...
- [ ] Pair input
  - [ ] ts_corr
  - [ ] ts_cov
  - [ ] ... Ref [qlib ops](https://github.com/microsoft/qlib/blob/main/qlib/data/ops.py)
