#pragma once
#include <cstdint>
#include <string>
#include <vector>
class FileLoader {
public:
  static std::vector<uint32_t> LoadBinaryFile(const std::string &path);
};
