find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

set(shader_source "${CMAKE_CURRENT_SOURCE_DIR}/shaders/test.comp")
set(shader_binary "${CMAKE_CURRENT_BINARY_DIR}/shaders/test.comp.spv")

add_custom_command(
    OUTPUT "${shader_binary}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory
            "${CMAKE_CURRENT_BINARY_DIR}/shaders"
    COMMAND "${GLSLANG_VALIDATOR}" -V "${shader_source}" -o "${shader_binary}"
    DEPENDS "${shader_source}"
    COMMENT "Compiling gpu_compute shader"
    VERBATIM
)

add_executable(gpu_compute
    main.cpp
    engine.cpp
    "${shader_binary}"
)

target_link_libraries(gpu_compute
    PRIVATE
        vulkan_base::engine
        fmt::fmt
)

target_compile_definitions(gpu_compute
    PRIVATE GPU_COMPUTE_SHADER_PATH="${shader_binary}"
)

vulkan_base_enable_warnings(gpu_compute)
