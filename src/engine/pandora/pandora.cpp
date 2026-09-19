#include "engine/pandora/pandora.hpp"
#include "VkBootstrap.h"
#include "engine/shaders/file_loader.hpp"
#include "engine/vk_toolkit/vk_toolkit.hpp"
#include "fmt/base.h"
#include "vk_mem_alloc.h"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

void Pandora::Init(unsigned long bufferSize, const std::string &shaderPath) {
  this->bufferSize = bufferSize;

  initInstance();
  initDescriptor();
  initBuffers();
  initPipeline(shaderPath);
  initCommands();
}

void Pandora::Upload(std::vector<int> a, int b, std::vector<int> c) {
  this->b = b;
  vmaCopyMemoryToAllocation(allocator, a.data(), storageAllocations[0], 0,
                            bufferSize);
  vmaCopyMemoryToAllocation(allocator, c.data(), storageAllocations[1], 0,
                            bufferSize);
}

std::vector<int> Pandora::Download() {
  std::vector<int> result(bufferSize / sizeof(int));
  vmaCopyAllocationToMemory(allocator, storageAllocations[2], 0, result.data(),
                            bufferSize);
  return result;
}

void Pandora::Dispatch(uint32_t x, uint32_t y, uint32_t z) {
  // reset command buffer
  vkResetCommandBuffer(commandBuffer, 0);

  auto beginInfo = VKToolkit::CommandBufferBeginInfo();
  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  // RECORDING START
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    calculationPipeline);
  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          calculationPipelineLayout, 0, 1, &descriptorSet, 0,
                          nullptr);
  vkCmdPushConstants(commandBuffer, calculationPipelineLayout,
                     VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(int), &b);

  vkCmdDispatch(commandBuffer, x, y, z);
  // RECORDING END

  vkEndCommandBuffer(commandBuffer);
  auto commandSubmitInfo = VKToolkit::CommandBufferSubmitInfo(commandBuffer);
  auto submitInfo =
      VKToolkit::SubmitInfo2(&commandSubmitInfo, nullptr, nullptr);
  vkQueueSubmit2(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);
}

void Pandora::Cleanup() { garbageCollector.Flush(); }

void Pandora::initInstance() {
  vkb::InstanceBuilder builder;

  vkb::Result<vkb::Instance> instanceRet = builder.use_default_debug_messenger()
                                               .request_validation_layers()
                                               .set_headless()
                                               .require_api_version(1, 3, 0)
                                               .build();
  if (!instanceRet) {
    fmt::println("{}", instanceRet.error().message());
    throw instanceRet.error();
  }
  instance = instanceRet.value();

  // vulkan 1.3 features
  VkPhysicalDeviceVulkan13Features features{
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
  features.dynamicRendering = true;
  features.synchronization2 = true;

  // vulkan 1.2 features
  VkPhysicalDeviceVulkan12Features features12{
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
  features12.bufferDeviceAddress = true;
  features12.descriptorIndexing = true;

  vkb::PhysicalDeviceSelector selector{instance};
  vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 3)
                                           .set_required_features_13(features)
                                           .set_required_features_12(features12)
                                           .select()
                                           .value();

  vkb::DeviceBuilder deviceBuilder{physicalDevice};
  auto deviceBuilderRet = deviceBuilder.build();

  device = deviceBuilderRet.value();
  choosenPhysicalDevice = physicalDevice.physical_device;

  graphicsQueue = device.get_queue(vkb::QueueType::graphics).value();
  graphicsQueueFamily =
      device.get_queue_index(vkb::QueueType::graphics).value();

  VmaAllocatorCreateInfo allocatorInfo = {};
  allocatorInfo.physicalDevice = choosenPhysicalDevice;
  allocatorInfo.device = device;
  allocatorInfo.instance = instance;
  allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  vmaCreateAllocator(&allocatorInfo, &allocator);

  garbageCollector.AddFunction([&]() { vmaDestroyAllocator(allocator); });
}

void Pandora::initDescriptor() {
  // Create Descriptor pool to store the input data
  VkDescriptorPoolSize poolSize = {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 4};
  VkDescriptorPoolCreateInfo descriptorPoolCreateInfo =
      VKToolkit::DescriptorPoolCreateInfo(1, 1, &poolSize);
  vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr,
                         &descriptorPool);
  garbageCollector.AddFunction(
      [&]() { vkDestroyDescriptorPool(device, descriptorPool, nullptr); });

  // Create the layout for storing the input in this case (a + b) input a and
  // input b
  VkDescriptorSetLayoutBinding bindings[3] = {
      VKToolkit::DescriptorSetLayoutBinding(
          0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT),
      VKToolkit::DescriptorSetLayoutBinding(
          1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT),
      VKToolkit::DescriptorSetLayoutBinding(
          2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT),
  };
  // = {0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
  //                                   1, VK_SHADER_STAGE_ALL, nullptr};
  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo =
      VKToolkit::DescriptorSetLayoutCreateInfo(3, bindings);
  vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr,
                              &descriptorSetLayout);
  garbageCollector.AddFunction([&]() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  });

  // Allocate the layout to the pool
  VkDescriptorSetAllocateInfo allocateInfo =
      VKToolkit::DescriptorSetAllocateInfo(1, &descriptorSetLayout,
                                           descriptorPool);
  vkAllocateDescriptorSets(device, &allocateInfo, &descriptorSet);
}

void Pandora::initPipeline(const std::string &shaderPath) {
  VkPushConstantRange pushRange{};
  pushRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  pushRange.offset = 0;
  pushRange.size = sizeof(int); // b is an int

  // Create pipeline layout with descriptor set
  auto computeLayout =
      VKToolkit::PipelineLayoutCreateInfo(&descriptorSetLayout);
  computeLayout.pushConstantRangeCount = 1;
  computeLayout.pPushConstantRanges = &pushRange;
  vkCreatePipelineLayout(device, &computeLayout, nullptr,
                         &calculationPipelineLayout);

  // Load compiled shader
  auto buffer = FileLoader::LoadBinaryFile(shaderPath);
  // Create shader module
  VkShaderModuleCreateInfo shaderModuleCreateInfo =
      VKToolkit::ShaderModuleCreateInfo(buffer);

  VkShaderModule computeShaderModule;
  vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr,
                       &computeShaderModule);

  // Create shader stage
  auto computeShaderStage = VKToolkit::PipelineShaderStageCreateInfo(
      computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT, "main");

  // Create compute pipeline
  auto computePipelineCreateInfo = VKToolkit::ComputePipelineCreateInfo(
      calculationPipelineLayout, computeShaderStage);
  vkCreateComputePipelines(device, VK_NULL_HANDLE, 1,
                           &computePipelineCreateInfo, nullptr,
                           &calculationPipeline);

  // Shader module is not needed anymore after the creation of the pipeline
  vkDestroyShaderModule(device, computeShaderModule, nullptr);
  garbageCollector.AddFunction([&]() {
    vkDestroyPipelineLayout(device, calculationPipelineLayout, nullptr);
    vkDestroyPipeline(device, calculationPipeline, nullptr);
  });
}

void Pandora::initCommands() {
  auto commandPoolInfo = VKToolkit::CommandPoolCreateInfo(
      graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool);

  auto commandBufferAllocateInfo =
      VKToolkit::CommandBufferAllocateInfo(commandPool);
  vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);

  garbageCollector.AddFunction(
      [&]() { vkDestroyCommandPool(device, commandPool, nullptr); });
}

void Pandora::initBuffers() {
  VmaAllocationCreateInfo allocInfos[3] = {
      {.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
       .usage = VMA_MEMORY_USAGE_AUTO},
      {.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
       .usage = VMA_MEMORY_USAGE_AUTO},
      {.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
                VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
       .usage = VMA_MEMORY_USAGE_AUTO},
  };

  for (size_t i = 0; i < 3; i++) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    bufferInfo.size = bufferSize;

    vmaCreateBuffer(allocator, &bufferInfo, &allocInfos[i], &storageBuffers[i],
                    &storageAllocations[i], nullptr);
    auto descriptorBufferInfo =
        VKToolkit::DescriptorBufferInfo(storageBuffers[i], 0, VK_WHOLE_SIZE);
    auto writeDescriptorSet =
        VKToolkit::WriteDescriptorSet(descriptorSet, &descriptorBufferInfo, i);

    vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
  }
  garbageCollector.AddFunction([&]() {
    for (size_t i = 0; i < 3; i++) {
      vmaDestroyBuffer(allocator, storageBuffers[i], storageAllocations[i]);
    }
  });
}
