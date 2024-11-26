#pragma once

// Standard Library

#include <bits/stdc++.h>

// Dependencies

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <glslang_c_interface.h>
#include <glslang_c_shader_types.h>
#include <resource_limits_c.h>
#include <spirv_cross.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Types

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef float    float32;
typedef double   float64;

// Logs

#define LOG_INFO(x) 	std::cout << "\033[1;90m[INFO]\033[0m\t\t" << x << "\033[0m" << std::endl;
#define LOG_SUCCESS(x)	std::cout << "\033[1;32m[SUCCESS]\033[0m\t" << x << "\033[0m" << std::endl;
#define LOG_WARNING(x)	std::cout << "\033[1;33m[WARNING]\033[0m\t" << x << "\033[0m" << std::endl;
#define LOG_ERROR(x)	std::cout << "\033[1;31m[ERROR]\033[0m\t\t" << x << "\033[0m" << std::endl;
