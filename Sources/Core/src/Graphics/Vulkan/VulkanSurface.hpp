//
// Created by acoezard on 11/5/24.
//

#pragma once

#include <vulkan/vulkan.h>

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanSurface
	{
		ApplicationContext* m_context = nullptr;

		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	public:
		VulkanSurface() = default;
		explicit VulkanSurface(ApplicationContext* context);
		~VulkanSurface();

		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface& operator=(const VulkanSurface&) = delete;
		VulkanSurface(VulkanSurface&& other) = delete;
		VulkanSurface& operator=(VulkanSurface&& other) = delete;

		[[nodiscard]]
		VkSurfaceKHR Handle() const;
	};
};
