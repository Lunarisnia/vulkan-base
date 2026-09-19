add_library(vulkan_base_engine
    engine/pandora/pandora.cpp
    engine/vma_usage.cpp
    engine/garbage_collector/garbage_collector.cpp
    engine/vk_toolkit/vk_toolkit.cpp
    engine/shaders/file_loader.cpp
)

add_library(vulkan_base::engine ALIAS vulkan_base_engine)

target_compile_features(vulkan_base_engine PUBLIC cxx_std_23)

target_include_directories(vulkan_base_engine
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(vulkan_base_engine SYSTEM
    PUBLIC
        $<BUILD_INTERFACE:${vma_SOURCE_DIR}/include>
)

target_link_libraries(vulkan_base_engine
    PUBLIC
        Vulkan::Vulkan
        vk-bootstrap::vk-bootstrap
        GPUOpen::VulkanMemoryAllocator
    PRIVATE
        fmt::fmt
)

vulkan_base_enable_warnings(vulkan_base_engine)
