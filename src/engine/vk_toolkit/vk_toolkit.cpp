
#include "engine/vk_toolkit/vk_toolkit.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>
VkDescriptorPoolCreateInfo VKToolkit::DescriptorPoolCreateInfo(int maxSets, int poolSizeCount,
                                                               VkDescriptorPoolSize* poolSize,
                                                               unsigned int flags) {
    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.flags = flags;
    poolCreateInfo.maxSets = maxSets;
    poolCreateInfo.poolSizeCount = poolSizeCount;
    poolCreateInfo.pPoolSizes = poolSize;
    return poolCreateInfo;
}

VkDescriptorSetLayoutCreateInfo
VKToolkit::DescriptorSetLayoutCreateInfo(int bindingCount, VkDescriptorSetLayoutBinding* binding,
                                         unsigned int flags) {
    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.flags = flags;
    createInfo.bindingCount = bindingCount;
    createInfo.pBindings = binding;
    return createInfo;
}

VkDescriptorSetAllocateInfo
VKToolkit::DescriptorSetAllocateInfo(int descriptorSetCount,
                                     VkDescriptorSetLayout* descriptorSetLayout,
                                     VkDescriptorPool descriptorPool) {
    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPool;
    allocateInfo.descriptorSetCount = descriptorSetCount;
    allocateInfo.pSetLayouts = descriptorSetLayout;
    return allocateInfo;
}

VkShaderModuleCreateInfo VKToolkit::ShaderModuleCreateInfo(std::vector<uint32_t>& codeBuffer,
                                                           unsigned int flags) {
    uint32_t codeSize = codeBuffer.size() * sizeof(uint32_t);
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.flags = flags;
    createInfo.pNext = nullptr;
    createInfo.codeSize = codeSize;
    createInfo.pCode = codeBuffer.data();
    return createInfo;
}

VkPipelineLayoutCreateInfo
VKToolkit::PipelineLayoutCreateInfo(VkDescriptorSetLayout* descriptorSetLayout,
                                    unsigned int flags) {
    VkPipelineLayoutCreateInfo createInfo = {};
    createInfo.flags = flags;
    createInfo.pNext = nullptr;
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pSetLayouts = descriptorSetLayout;
    createInfo.setLayoutCount = 1; // For now
    return createInfo;
}

VkPipelineShaderStageCreateInfo
VKToolkit::PipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage,
                                         const std::string& pName, unsigned int flags) {
    VkPipelineShaderStageCreateInfo stageInfo{};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.pNext = nullptr;
    stageInfo.flags = flags;
    stageInfo.stage = stage;
    stageInfo.module = shaderModule;
    stageInfo.pName = pName.c_str();
    return stageInfo;
}

VkComputePipelineCreateInfo
VKToolkit::ComputePipelineCreateInfo(VkPipelineLayout pipelineLayout,
                                     VkPipelineShaderStageCreateInfo stage, unsigned int flags) {
    VkComputePipelineCreateInfo computePipelineCreateInfo = {};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.pNext = nullptr;
    computePipelineCreateInfo.flags = flags;
    computePipelineCreateInfo.layout = pipelineLayout;
    computePipelineCreateInfo.stage = stage;
    return computePipelineCreateInfo;
}

VkCommandPoolCreateInfo VKToolkit::CommandPoolCreateInfo(uint32_t queueFamilyIndex,
                                                         unsigned int flags) {
    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = flags;
    createInfo.queueFamilyIndex = queueFamilyIndex;
    return createInfo;
}

VkCommandBufferAllocateInfo VKToolkit::CommandBufferAllocateInfo(VkCommandPool commandPool,
                                                                 unsigned int) {
    VkCommandBufferAllocateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.commandPool = commandPool;
    createInfo.commandBufferCount = 1; // For now
    createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    return createInfo;
}

VkCommandBufferBeginInfo VKToolkit::CommandBufferBeginInfo(unsigned int flags) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.pNext = nullptr;
    return beginInfo;
}

VkSubmitInfo2 VKToolkit::SubmitInfo2(VkCommandBufferSubmitInfo* cmd,
                                     VkSemaphoreSubmitInfo* signalSemaphoreInfo,
                                     VkSemaphoreSubmitInfo* waitSemaphoreInfo) {
    VkSubmitInfo2 info{};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    info.pNext = nullptr;

    info.waitSemaphoreInfoCount = waitSemaphoreInfo == nullptr ? 0 : 1;
    info.pWaitSemaphoreInfos = waitSemaphoreInfo;

    info.signalSemaphoreInfoCount = signalSemaphoreInfo == nullptr ? 0 : 1;
    info.pSignalSemaphoreInfos = signalSemaphoreInfo;

    info.commandBufferInfoCount = 1;
    info.pCommandBufferInfos = cmd;
    return info;
}

VkCommandBufferSubmitInfo VKToolkit::CommandBufferSubmitInfo(VkCommandBuffer commandBuffer) {
    VkCommandBufferSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.commandBuffer = commandBuffer;
    submitInfo.deviceMask = 0;
    return submitInfo;
}

VkDescriptorBufferInfo VKToolkit::DescriptorBufferInfo(VkBuffer buffer, VkDeviceSize offset,
                                                       VkDeviceSize range) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = offset;
    bufferInfo.range = range;
    return bufferInfo;
}

VkWriteDescriptorSet VKToolkit::WriteDescriptorSet(VkDescriptorSet descriptorSet,
                                                   VkDescriptorBufferInfo* bufferInfo,
                                                   uint32_t binding) {
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptorSet;
    write.dstBinding = binding;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.pBufferInfo = bufferInfo;
    return write;
}

VkDescriptorSetLayoutBinding VKToolkit::DescriptorSetLayoutBinding(uint32_t binding,
                                                                   VkDescriptorType descriptorType,
                                                                   uint32_t descriptorCount,
                                                                   VkShaderStageFlags stageFlags) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.stageFlags = stageFlags;
    layoutBinding.pImmutableSamplers = nullptr;

    return layoutBinding;
}
