
clean:
	rm -rf build
	
test: clean
	mkdir build && cd build && cmake .. && make -j4
	cd ./build/optimus && ctest -V

bench: clean
	mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j4
	./build/optimus/benchmark_ts_sum
