.PHONY: run
run: build
	./build/apps/gpu_compute/gpu_compute

.PHONY: run-renderer
run-renderer: build-renderer
	./build/apps/renderer/renderer

.PHONY: build
build:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./build --target gpu_compute

.PHONY: build-renderer
build-renderer:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./build --target renderer
