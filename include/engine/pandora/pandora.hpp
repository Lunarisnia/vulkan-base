#pragma once
#include "SDL3/SDL_video.h"
#include "VkBootstrap.h"
#include "engine/garbage_collector/garbage_collector.hpp"
#include "vk_mem_alloc.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class Pandora {
    struct FrameData {
        VkCommandPool mainCommandPool{VK_NULL_HANDLE};
        VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
        VkSemaphore imageAvailableSemaphore{VK_NULL_HANDLE};
        VkSemaphore renderFinishedSemaphore{VK_NULL_HANDLE};
        VkFence renderFence{VK_NULL_HANDLE};
    };

  private:
    GarbageCollector garbageCollector;
    vkb::Instance instance;
    vkb::Device device;
    VkPhysicalDevice choosenPhysicalDevice;

    SDL_Window* window;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamily;

    VkQueue presentQueue;
    uint32_t presentQueueFamily;

    VmaAllocator allocator;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;

    VkPipelineLayout calculationPipelineLayout;
    VkPipeline calculationPipeline;

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    int frameIndex = 0;
    std::array<FrameData, 2> frames;

    unsigned long bufferSize;
    VkBuffer storageBuffers[3];
    VmaAllocation storageAllocations[3];

    int b;

  private:
    void initInstance(bool headless);
    void initSwapchain();
    void initDescriptor();
    void initBuffers();
    void initPipeline(const std::string& shaderPath);
    void initCommands();
    void initSync();

    void draw();

  public:
    void Init();
    void Run();
    void InitHeadless(unsigned long bufferSize, const std::string& shaderPath);
    void Upload(std::vector<int> a, int b, std::vector<int> c);
    std::vector<int> Download();
    void Dispatch(uint32_t x, uint32_t y, uint32_t z);

    void Cleanup();
};
