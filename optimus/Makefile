
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


test_ts_mad:
	rm -rf ./ts_mad_test
	$(CC) $(CXXFLAGS) $(OPTFLAGS) -lgtest -lpthread ts_mad.cpp ts_mad_test.cpp -o ts_mad_test
	./ts_mad_test
	rm ./ts_mad_test


bench_ts_mad:
	rm -rf ./ts_mad_benchmark
	$(CC) $(CXXFLAGS) $(OPTFLAGS) -lbenchmark -lpthread ts_mad.cpp ts_mad_benchmark.cpp -o ts_mad_benchmark
	./ts_mad_benchmark
	rm ./ts_mad_benchmark
