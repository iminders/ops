
CC = g++

cpu_opt_flag=-march=native
UNAME_S := $(shell uname -m)
ifeq ($(UNAME_S), arm64)
	cpu_opt_flag=-mcpu=apple-m1
endif


ts_sum_test:
	$(CC) $(cpu_opt_flag) -o3 -std=c++17 -I/usr/local/lib -lgtest -lpthread ts_sum.cpp ts_sum_test.cpp -o ts_sum_test
	./ts_sum_test
	rm ./ts_sum_test


ts_sum_bench:
	$(CC) $(cpu_opt_flag) -o3 -std=c++14 -I/usr/local/lib -lbenchmark -lpthread ts_sum.cpp ts_sum_benchmark.cpp -o ts_sum_benchmark
	./ts_sum_benchmark
	rm ./ts_sum_benchmark

