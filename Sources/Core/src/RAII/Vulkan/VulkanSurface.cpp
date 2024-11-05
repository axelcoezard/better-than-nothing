//
// Created by acoezard on 11/5/24.
//

#include "VulkanSurface.hpp"

#include "ApplicationContext.hpp"
#include "VulkanInstance.hpp"

namespace BetterThanNothing
{
	VulkanSurface::VulkanSurface(const VulkanInstance& instance, const GlfwWindow& window): m_instance(instance.Handle())
	{
		if (glfwCreateWindowSurface(instance.Handle(), window.Handle(), nullptr, &m_surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	}
};


