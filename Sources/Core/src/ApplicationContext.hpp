//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "RAII/Window.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"

namespace BetterThanNothing
{
	struct ApplicationContext
	{
		Window window;
		VulkanInstance vulkanInstance;
	};
} // BetterThanNothing