# ops
Realtime stream op accelerator [![UnitTest](https://github.com/iminders/ops/actions/workflows/unittest.yml/badge.svg)](https://github.com/iminders/ops/actions/workflows/unittest.yml)

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

### Run tests
ts_sum example
```
cd optimus
# unit test
make ts_sum_test
# benchmark
make ts_sum_bench
```