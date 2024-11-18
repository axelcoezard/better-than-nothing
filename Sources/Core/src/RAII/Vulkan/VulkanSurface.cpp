//
// Created by acoezard on 11/5/24.
//

#include "VulkanSurface.hpp"

#include "ApplicationContext.hpp"
#include "VulkanInstance.hpp"

namespace BetterThanNothing
{
	VulkanSurface::VulkanSurface(ApplicationContext* context): m_context(context)
	{
		if (glfwCreateWindowSurface(m_context->GetVulkanInstance()->Handle(), m_context->GetWindow()->Handle(), nullptr, &m_surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface");

		LOG_SUCCESS("Vulkan surface: ok");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_context->GetVulkanInstance()->Handle(), m_surface, nullptr);
	}

	VkSurfaceKHR VulkanSurface::Handle() const
	{
		if (m_surface == VK_NULL_HANDLE)
			throw std::runtime_error("VulkanSurface::Handle() Vulkan surface is not initialized");
		return m_surface;
	}

	VulkanSurface::operator VkSurfaceKHR() const
	{
		return m_surface;
	}
};


