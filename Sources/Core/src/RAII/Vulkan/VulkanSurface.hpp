//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "RAII/Vulkan/VulkanInstance.hpp"
#include "RAII/Glfw/GlfwWindow.hpp"

namespace BetterThanNothing
{
	class VulkanSurface
	{
		VkInstance m_instance;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	public:
		explicit VulkanSurface(const VulkanInstance& instance, const GlfwWindow& window);
		~VulkanSurface();

		// Prevent copy to ensure unique ownership of nodes
		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface& operator=(const VulkanSurface&) = delete;

		// Allow move semantics
		VulkanSurface(VulkanSurface&& other) noexcept;
		VulkanSurface& operator=(VulkanSurface&& other) noexcept;

		VkSurfaceKHR Handle() const;

		explicit operator VkSurfaceKHR() const;
	};
};
