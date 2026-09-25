#include "engine/garbage_collector/garbage_collector.hpp"
#include <functional>

void GarbageCollector::AddFunction(std::function<void()> deletionFunc) {
    deletionQueue.emplace_back(deletionFunc);
}

void GarbageCollector::Flush() {
    for (auto it = deletionQueue.rbegin(); it != deletionQueue.rend(); ++it) {
        (*it)();
    }
    deletionQueue.clear();
}
