cmake_minimum_required(VERSION 3.8)

project("Core")

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB SOURCES
	"src/*.cpp"
	"src/*.hpp"
	"src/*.h"
	"src/Utils/*.cpp"
	"src/Utils/*.hpp"
	"src/Events/*.hpp"
	"src/Events/*.cpp"
)

include_directories(src)

add_library(${PROJECT_NAME} ${SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE ../pch.hpp)

target_compile_definitions(Core PRIVATE ENABLE_IMGUI=1)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm vulkan stb imgui inih glslang SPIRV glslang-default-resource-limits spirv_cross GPUOpen::VulkanMemoryAllocator)

target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/glfw/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/vulkan)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/glm/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/stb/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/tinyobjloader)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/entt/single_include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/imgui/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/inih/include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/glslang/glslang/Include)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/glslang/glslang/Public)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/spirv_cross/spirv_cross)
target_include_directories(${PROJECT_NAME} PRIVATE ../../ThirdParty/vma/include)

target_include_directories(${PROJECT_NAME} PRIVATE src)

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include)
