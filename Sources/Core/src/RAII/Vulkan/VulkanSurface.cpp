//
// Created by acoezard on 11/5/24.
//

#include "VulkanSurface.hpp"

#include "ApplicationContext.hpp"
#include "VulkanInstance.hpp"

namespace BetterThanNothing
{
	VulkanSurface::VulkanSurface(ApplicationContext* context)
		: m_instance(context->GetVulkanInstance()->Handle())
	{
		if (glfwCreateWindowSurface(m_instance, context->GetWindow()->Handle(), nullptr, &m_surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface");

		LOG_SUCCESS("Vulkan surface: ok");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	}

	VulkanSurface::VulkanSurface(VulkanSurface&& other) noexcept
	{
		_swap(std::move(other));
	}

	VulkanSurface& VulkanSurface::operator=(VulkanSurface&& other) noexcept
	{
		if (this != &other)
			_swap(std::move(other));
		return *this;
	}

	void VulkanSurface::_swap(VulkanSurface&& other)
	{
		if (m_surface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

		m_surface = other.m_surface;
		other.m_surface = VK_NULL_HANDLE;
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


