#pragma once
#include "VkBootstrap.h"
#include "engine/garbage_collector/garbage_collector.hpp"
#include "vk_mem_alloc.h"
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class Pandora {
private:
  GarbageCollector garbageCollector;
  vkb::Instance instance;
  vkb::Device device;
  VkPhysicalDevice choosenPhysicalDevice;

  VkQueue graphicsQueue;
  uint32_t graphicsQueueFamily;

  VmaAllocator allocator;

  VkDescriptorPool descriptorPool;
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorSet descriptorSet;

  VkPipelineLayout calculationPipelineLayout;
  VkPipeline calculationPipeline;

  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;

  unsigned long bufferSize;
  VkBuffer storageBuffers[3];
  VmaAllocation storageAllocations[3];

  int b;

private:
  void initInstance(bool headless);
  void initDescriptor();
  void initBuffers();
  void initPipeline(const std::string &shaderPath);
  void initCommands();

public:
  void Init();
  void InitHeadless(unsigned long bufferSize, const std::string &shaderPath);
  void Upload(std::vector<int> a, int b, std::vector<int> c);
  std::vector<int> Download();
  void Dispatch(uint32_t x, uint32_t y, uint32_t z);

  void Cleanup();
};
