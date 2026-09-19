#include "engine/garbage_collector/garbage_collector.hpp"
#include <functional>

void GarbageCollector::AddFunction(std::function<void()> deletionFunc) {
  deletionQueue.emplace_back(deletionFunc);
}

void GarbageCollector::Flush() {
  for (int i = deletionQueue.size() - 1; i >= 0; i--) {
    deletionQueue[i]();
  }
  deletionQueue.clear();
}
