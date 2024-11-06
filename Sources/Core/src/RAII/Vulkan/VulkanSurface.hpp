//
// Created by acoezard on 11/5/24.
//

#pragma once

namespace BetterThanNothing
{
	struct ApplicationContext;

	class VulkanSurface
	{
		VkInstance m_instance = VK_NULL_HANDLE;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	public:
		explicit VulkanSurface(const ApplicationContext* context);
		~VulkanSurface();

		// Prevent copy to ensure unique ownership of nodes
		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface& operator=(const VulkanSurface&) = delete;

		// Allow move semantics
		VulkanSurface(VulkanSurface&& other) noexcept;
		VulkanSurface& operator=(VulkanSurface&& other) noexcept;

		VkSurfaceKHR Handle() const;

		explicit operator VkSurfaceKHR() const;

	private:
		void _swap(VulkanSurface&& other);
	};
};
