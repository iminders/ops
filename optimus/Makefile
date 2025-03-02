
CC = g++

CXXFLAGS += -std=c++14 -Wall -pthread 

CPUFLAG = -march=native
UNAME_S := $(shell uname -m)
ifeq ($(UNAME_S), arm64)
	CPUFLAG = -mcpu=apple-m1
endif
OPTFLAGS = $(CPUFLAG) -o3


hello:
	$(CC) $(CXXFLAGS) -lgtest -lpthread hello.cpp -o hello
	./hello
	rm ./hello

ts_sum_test:
	$(CC) $(CXXFLAGS) -lgtest ts_sum.cpp ts_sum_test.cpp -o ts_sum_test
	./ts_sum_test
	rm ./ts_sum_test

ts_sum_bench:
	$(CC) $(CXXFLAGS) $(OPTFLAGS) -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark
	./ts_sum_benchmark
	rm ./ts_sum_benchmark

