#include "engine/shaders/file_loader.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<uint32_t> FileLoader::LoadBinaryFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

  file.seekg(0);
  file.read((char *)buffer.data(), fileSize);
  file.close();

  return buffer;
}
