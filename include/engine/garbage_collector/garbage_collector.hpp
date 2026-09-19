#pragma once

#include <functional>
#include <vector>
class GarbageCollector {
private:
  std::vector<std::function<void()>> deletionQueue;

public:
  void AddFunction(std::function<void()> deletionFunc);

  void Flush();
};
