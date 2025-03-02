# Minimum required CMake version for this project
cmake_minimum_required(VERSION 3.14)

# Project name and basic configuration
project(ops)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译器优化配置
if(CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
    message(STATUS "Applying optimization flags for ${CMAKE_BUILD_TYPE} build")

    # 通用优化选项
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:CXX>:-O3>"
        "$<$<COMPILE_LANGUAGE:CXX>:-ffast-math>"
    )

    # 架构特定优化
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # 检测 Apple Silicon
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" AND APPLE)
            message(STATUS "Applying Apple M1 optimization flags")
            add_compile_options(-mcpu=apple-m1)
        else()
            # 其他平台使用 march=native
            add_compile_options(-march=native)
        endif()
    elseif(MSVC)
        add_compile_options(/O2 /fp:fast)
    endif()

    # 链接时优化
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Google Test configuration 
# Tries to find system-wide GoogleTest first
find_package(GTest CONFIG QUIET)

if (TARGET GTest::gtest AND TARGET GTest::gtest_main)
    message(STATUS "Found system GoogleTest")
else()
    message(STATUS "GoogleTest not found, fetching from source...")
    # Fetch GoogleTest from repository if not found
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Google Benchmark configuration
# Tries to find system-wide Google Benchmark first
find_package(benchmark CONFIG QUIET)

if (TARGET benchmark::benchmark AND TARGET benchmark::benchmark_main)
    message(STATUS "Found system Google Benchmark")
else()
    message(STATUS "Google Benchmark not found, fetching from source...")
    # Fetch Google Benchmark from repository if not found
    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
    )
    FetchContent_MakeAvailable(benchmark)
endif()


add_subdirectory(optimus)