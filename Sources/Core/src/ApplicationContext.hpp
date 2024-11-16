//
// Created by acoezard on 11/5/24.
//

#pragma once

#include <utility>

#include "RAII/Window.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"
#include "RAII/Vulkan/VulkanSurface.hpp"
#include "RAII/Vulkan/VulkanDevice.hpp"

namespace BetterThanNothing
{
	// TODO: rework the application context to be more flexible
	// => maybe use a builder pattern to create the context
	// => could first set parameters (validation layers, etc) and then build the context

	struct ApplicationContext
	{
		bool running = true;

		Window* window = nullptr;
		VulkanInstance* vulkanInstance = nullptr;
		VulkanSurface* vulkanSurface = nullptr;
		VulkanDevice* vulkanDevice = nullptr;

		VkQueue graphicsQueue = VK_NULL_HANDLE;

		explicit ApplicationContext() = default;

		~ApplicationContext()
		{
			delete vulkanSurface;
			delete vulkanDevice;
			delete vulkanInstance;
			delete window;
		}

		void InitWindow(const std::string& title, int32_t width, int32_t height, bool fullscreen = false, bool resizable = false)
		{
			window = new Window(title, width, height, fullscreen, resizable);
		}

		void InitVulkan(bool enableValidationLayers)
		{
			vulkanInstance = new VulkanInstance(enableValidationLayers);
			vulkanDevice = new VulkanDevice(this);
			vulkanSurface = new VulkanSurface(this);
		}
	};
} // BetterThanNothing