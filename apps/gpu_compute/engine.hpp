#pragma once

#include "engine/pandora/pandora.hpp"

class Engine {
private:
  Pandora pandora;

public:
  void Init();
  void Run();
  ~Engine();
};
