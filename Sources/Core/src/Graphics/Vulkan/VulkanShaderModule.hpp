#pragma once

#include "VulkanShaderType.hpp"

namespace BetterThanNothing
{
	struct VulkanShaderModule
	{
		std::string shaderPath;

		VulkanShaderType shaderType;

		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};
};
