#pragma once

#include "VulkanShaderType.hpp"

namespace BetterThanNothing
{
	struct VulkanShaderModule
	{
		std::string filename;

		VulkanShaderType shaderType;

		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};
};
