//
// Created by acoezard on 11/5/24.
//

#include "VulkanInstance.hpp"
#include <stdexcept>

namespace BetterThanNothing
{
	VulkanInstance::VulkanInstance(const bool enableValidationLayers)
	{
		(void) enableValidationLayers;
	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	VulkanInstance::VulkanInstance(VulkanInstance&& other) noexcept
	{
		_move(std::move(other));
	}

	VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other) noexcept
	{
		if (this != &other)
			_move(std::move(other));
		return *this;
	}

	void VulkanInstance::_move(VulkanInstance&& other) noexcept
	{
		if (m_instance != VK_NULL_HANDLE)
			vkDestroyInstance(m_instance, nullptr);

		m_instance = other.m_instance;
		other.m_instance = VK_NULL_HANDLE;
	}

	VkInstance VulkanInstance::Handle() const
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");
		return m_instance;
	}

	VulkanInstance::operator VkInstance() const
	{
		return m_instance;
	}

}


