include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.2.0
    EXCLUDE_FROM_ALL
)

FetchContent_Declare(
    vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
    GIT_TAG v1.3.302
    EXCLUDE_FROM_ALL
)

FetchContent_Declare(
    vma
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG v3.2.1
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(fmt vk_bootstrap vma)
