#pragma once

// Standard Library

#include <bits/stdc++.h>

// Dependencies

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vk_mem_alloc.h>

#include <glslang_c_interface.h>
#include <glslang_c_shader_types.h>
#include <resource_limits_c.h>
#include <spirv_cross.hpp>

// LOGS

#define LOG_INFO(x) 	std::cout << "\033[1;90m[INFO]\033[0m\t\t" << x << "\033[0m" << std::endl;
#define LOG_SUCCESS(x)	std::cout << "\033[1;32m[SUCCESS]\033[0m\t" << x << "\033[0m" << std::endl;
#define LOG_WARNING(x)	std::cout << "\033[1;33m[WARNING]\033[0m\t" << x << "\033[0m" << std::endl;
#define LOG_ERROR(x)	std::cout << "\033[1;31m[ERROR]\033[0m\t\t" << x << "\033[0m" << std::endl;
