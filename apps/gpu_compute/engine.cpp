#include "engine.hpp"

#include "fmt/base.h"
#include <chrono>
#include <random>
#include <vector>

const int ELEMENT_COUNT = 30000000;

void Engine::Init() {
    pandora.InitHeadless(ELEMENT_COUNT * sizeof(int), GPU_COMPUTE_SHADER_PATH);
}

void Engine::Run() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 10000);

    std::vector<int> a;
    std::vector<int> c;
    fmt::println("Seeding data");
    const int report = 10000000;
    for (int i = 0; i < ELEMENT_COUNT; i++) {
        if (i % report == 0) {
            fmt::println("Parsed: {}", report * (i / report));
        }
        a.emplace_back(dist(rng));
        c.emplace_back(dist(rng));
    }
    fmt::println("Finished seeding data");

    fmt::println("Uploading to GPU");
    pandora.Upload(a, 2, c);
    auto start = std::chrono::high_resolution_clock::now();
    pandora.Dispatch(ELEMENT_COUNT, 1, 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto result = pandora.Download();
    fmt::println("Calculation took {}ms", ms);
}

Engine::~Engine() {
    pandora.Cleanup();
}
