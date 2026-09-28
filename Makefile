CONFIG ?= Debug
BUILD_DIR ?= build
CONFIGURE_OPTIONS := -G Ninja -DCMAKE_BUILD_TYPE=$(CONFIG)

ifeq ($(OS),Windows_NT)
EXE_SUFFIX := .exe
else
EXE_SUFFIX :=
endif

.PHONY: run
run: build
	./$(BUILD_DIR)/apps/gpu_compute/gpu_compute$(EXE_SUFFIX)

.PHONY: run-renderer
run-renderer: build-renderer
	./$(BUILD_DIR)/apps/renderer/renderer$(EXE_SUFFIX)

.PHONY: build
build:
	cmake -S . -B ./$(BUILD_DIR) $(CONFIGURE_OPTIONS)
	cmake --build ./$(BUILD_DIR) --target gpu_compute

.PHONY: build-renderer
build-renderer:
	cmake -S . -B ./$(BUILD_DIR) $(CONFIGURE_OPTIONS)
	cmake --build ./$(BUILD_DIR) --target renderer
