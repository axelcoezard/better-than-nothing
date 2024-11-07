//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "RAII/Window.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"
#include "RAII/Vulkan/VulkanSurface.hpp"
#include "RAII/Vulkan/VulkanPhysicalDevice.hpp"

namespace BetterThanNothing
{
	struct ApplicationContext
	{
		bool running = true;

		Window window;
		VulkanInstance vulkanInstance;
		VulkanSurface vulkanSurface = {};
		VulkanPhysicalDevice vulkanPhysicalDevice = {};
	};
} // BetterThanNothing