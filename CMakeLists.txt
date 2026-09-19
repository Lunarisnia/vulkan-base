cmake_minimum_required(VERSION 3.24)

project(vulkan_base
    VERSION 0.1.0
    DESCRIPTION "A Vulkan foundation for graphics experiments"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

option(VULKAN_BASE_BUILD_APPS "Build applications" ON)
option(VULKAN_BASE_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(VULKAN_BASE_ENABLE_SANITIZERS
       "Enable address and undefined behavior sanitizers" OFF)

include(cmake/CompilerWarnings.cmake)
include(cmake/Dependencies.cmake)

if(VULKAN_BASE_ENABLE_SANITIZERS
   AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(-g -O1 -fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

add_subdirectory(src)

if(VULKAN_BASE_BUILD_APPS)
    add_subdirectory(apps)
endif()
