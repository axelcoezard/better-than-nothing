# ==================================================================================================
# Global configuration
# ==================================================================================================
cmake_minimum_required(VERSION 3.8)

project("better-than-nothing")

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# ==================================================================================================
# Dependencies
# ==================================================================================================
add_subdirectory("ThirdParty/glfw")
add_subdirectory("ThirdParty/glm")
add_subdirectory("ThirdParty/vulkan")
add_subdirectory("ThirdParty/entt")
add_subdirectory("ThirdParty/imgui")
add_subdirectory("ThirdParty/stb")
add_subdirectory("ThirdParty/inih")
add_subdirectory("ThirdParty/glslang")
add_subdirectory("ThirdParty/spirv_cross")
add_subdirectory("ThirdParty/vma")
add_subdirectory("ThirdParty/openal")
add_subdirectory("ThirdParty/libsndfile")

# ==================================================================================================
# Sources
# ==================================================================================================
add_subdirectory("Sources/Core")
add_subdirectory("Sources/Runtime")




