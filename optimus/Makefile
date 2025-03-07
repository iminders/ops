
CC = g++

CXXFLAGS += -std=c++14 -Wall -pthread 

CPUFLAG = -march=native
UNAME_S := $(shell uname -m)
ifeq ($(UNAME_S), arm64)
	CPUFLAG = -mcpu=apple-m1
endif
OPTFLAGS = $(CPUFLAG) -O3


test_ts_sum:
	rm -rf ./ts_sum_test
	$(CC) $(CXXFLAGS) -lgtest ts_sum.cpp ts_sum_test.cpp -o ts_sum_test && ./ts_sum_test
	rm ./ts_sum_test

bench_ts_sum:
	rm -rf ./ts_sum_benchmark
	$(CC) $(CXXFLAGS) $(OPTFLAGS) -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark && ./ts_sum_benchmark
	rm ./ts_sum_benchmark

