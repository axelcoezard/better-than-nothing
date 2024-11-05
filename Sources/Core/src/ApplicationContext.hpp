//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "RAII/Glfw/GlfwWindow.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"

namespace BetterThanNothing
{
	struct ApplicationContext
	{
		GlfwWindow window;
		VulkanInstance vulkanInstance;
	};
} // BetterThanNothing