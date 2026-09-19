add_executable(renderer main.cpp)

target_link_libraries(renderer
    PRIVATE
        vulkan_base::engine
)

vulkan_base_enable_warnings(renderer)
