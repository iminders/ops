
clean:
	rm -rf build
	
test: clean
	mkdir build && cd build && cmake .. && make -j4
	cd ./build/optimus && ctest -V

prod: clean
	mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j4

bench: prod
	cd build && make run_all_benchmarks
