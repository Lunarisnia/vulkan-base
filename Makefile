.PHONY: run
run: build
	./build/apps/gpu_compute/gpu_compute

.PHONY: build
build:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./build --target gpu_compute
