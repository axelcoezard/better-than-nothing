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

	class ApplicationContext
	{
	private:
		bool running = true;

		std::unique_ptr<Window> window;
		std::unique_ptr<VulkanInstance> vulkanInstance;
		std::unique_ptr<VulkanSurface> vulkanSurface;
		std::unique_ptr<VulkanDevice> vulkanDevice;

	public:
		explicit ApplicationContext() = default;
		~ApplicationContext() = default;

		void InitWindow(const std::string& title, int32_t width, int32_t height, bool fullscreen = false, bool resizable = false)
		{
			window = std::make_unique<Window>(title, width, height, fullscreen, resizable);
		}

		void InitVulkan(bool enableValidationLayers)
		{
			vulkanInstance = std::make_unique<VulkanInstance>(enableValidationLayers);
			vulkanDevice = std::make_unique<VulkanDevice>(this);
			vulkanSurface = std::make_unique<VulkanSurface>(this);
		}

		bool IsRunning() const
		{
			return running;
		}

		void SetRunning(bool value)
		{
			running = value;
		}

		std::unique_ptr<Window>& GetWindow()
		{
			return window;
		}

		std::unique_ptr<VulkanInstance>& GetVulkanInstance()
		{
			return vulkanInstance;
		}

		std::unique_ptr<VulkanSurface>& GetVulkanSurface()
		{
			return vulkanSurface;
		}

		std::unique_ptr<VulkanDevice>& GetVulkanDevice()
		{
			return vulkanDevice;
		}
	};
} // BetterThanNothing