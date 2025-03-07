# Minimum required CMake version for this project
cmake_minimum_required(VERSION 3.14)

# Project name and basic configuration
project(ops)

set(CMAKE_CXX_STANDARD 14)
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



# 查找当前目录下的所有性能测试源文件
file(GLOB BENCHMARK_SOURCES "optimus/*_benchmark.cpp")

# 为每个性能测试文件创建可执行目标
foreach(BENCHMARK_SOURCE ${BENCHMARK_SOURCES})
    # 获取文件名（不包含路径和扩展名）
    get_filename_component(BENCHMARK_NAME ${BENCHMARK_SOURCE} NAME_WE)
    
    string(REPLACE "_benchmark" "_lib" BENCHMARK_LIB ${BENCHMARK_NAME} )
    
    # 创建可执行目标
    add_executable(${BENCHMARK_NAME} ${BENCHMARK_SOURCE})
    
    # 链接 Google Benchmark 库
    target_link_libraries(${BENCHMARK_NAME} PRIVATE ${BENCHMARK_LIB}  benchmark::benchmark)
endforeach()

# 可选：创建一个运行所有性能测试的目标
add_custom_target(run_all_benchmarks
    COMMAND ${CMAKE_COMMAND} -E echo "Running all benchmarks..."
)

# 为每个基准测试添加运行命令
foreach(BENCHMARK_SOURCE ${BENCHMARK_SOURCES})
    get_filename_component(BENCHMARK_NAME ${BENCHMARK_SOURCE} NAME_WE)
    
    add_custom_command(
        TARGET run_all_benchmarks POST_BUILD
        COMMAND ${BENCHMARK_NAME}
        COMMENT "Running benchmark: ${BENCHMARK_NAME}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()