#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class VKToolkit {
public:
  static VkDescriptorPoolCreateInfo
  DescriptorPoolCreateInfo(int maxSets, int poolSizeCount,
                           VkDescriptorPoolSize *poolSize,
                           unsigned int flags = 0);
  static VkDescriptorSetLayoutCreateInfo
  DescriptorSetLayoutCreateInfo(int bindingCount,
                                VkDescriptorSetLayoutBinding *binding,
                                unsigned int flags = 0);
  static VkDescriptorSetAllocateInfo
  DescriptorSetAllocateInfo(int descriptorSetCount,
                            VkDescriptorSetLayout *descriptorSetLayout,
                            VkDescriptorPool descriptorPool);
  static VkShaderModuleCreateInfo
  ShaderModuleCreateInfo(std::vector<uint32_t> &codeBuffer,
                         unsigned int flags = 0);
  static VkPipelineLayoutCreateInfo
  PipelineLayoutCreateInfo(VkDescriptorSetLayout *descriptorSetLayout,
                           unsigned int flags = 0);
  static VkComputePipelineCreateInfo
  ComputePipelineCreateInfo(VkPipelineLayout pipelineLayout,
                            VkPipelineShaderStageCreateInfo stage,
                            unsigned int flags = 0);
  static VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(
      VkShaderModule shaderModule, VkShaderStageFlagBits stage,
      const std::string &pName, unsigned int flags = 0);
  static VkCommandPoolCreateInfo
  CommandPoolCreateInfo(uint32_t queueFamilyIndex, unsigned int flags = 0);
  static VkCommandBufferAllocateInfo
  CommandBufferAllocateInfo(VkCommandPool commandPool, unsigned int flags = 0);
  static VkCommandBufferBeginInfo
  CommandBufferBeginInfo(unsigned int flags = 0);
  static VkSubmitInfo2 SubmitInfo2(VkCommandBufferSubmitInfo *cmd,
                                   VkSemaphoreSubmitInfo *signalSemaphoreInfo,
                                   VkSemaphoreSubmitInfo *waitSemaphoreInfo);
  static VkCommandBufferSubmitInfo
  CommandBufferSubmitInfo(VkCommandBuffer commandBuffer);

  static VkDescriptorBufferInfo DescriptorBufferInfo(VkBuffer buffer,
                                                     VkDeviceSize offset,
                                                     VkDeviceSize range);

  static VkWriteDescriptorSet
  WriteDescriptorSet(VkDescriptorSet descriptorSet,
                     VkDescriptorBufferInfo *bufferInfo, uint32_t binding);

  static VkDescriptorSetLayoutBinding
  DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType,
                             uint32_t descriptorCount,
                             VkShaderStageFlags stageFlags);
};
