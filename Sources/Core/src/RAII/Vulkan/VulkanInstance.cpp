//
// Created by acoezard on 11/5/24.
//

#include "VulkanInstance.hpp"

namespace BetterThanNothing
{
	VulkanInstance::VulkanInstance(const bool enableValidationLayers)
	{
		vkb::InstanceBuilder instance_builder;

		auto system_info_ret = vkb::SystemInfo::get_system_info();
		if (!system_info_ret) {
			throw std::runtime_error("Could not get system info");
		}

		auto system_info = system_info_ret.value();

		if (enableValidationLayers && system_info.validation_layers_available)
			instance_builder.request_validation_layers();

		instance_builder.use_default_debug_messenger();
		instance_builder.set_engine_name("BetterThanNothing");
		instance_builder.set_engine_version(1, 0, 0);
		instance_builder.require_api_version(1, 0);

		instance_builder.enable_extensions(_getRequiredExtensions());

		auto instance_builder_return = instance_builder.build();

		if (!instance_builder_return)
			throw std::runtime_error("Could not create Vulkan instance!");

		m_vkbInstance = instance_builder_return.value();

		if (m_vkbInstance == VK_NULL_HANDLE || m_vkbInstance.instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");
	}

	VulkanInstance::~VulkanInstance()
	{
		if (m_vkbInstance != VK_NULL_HANDLE)
			vkb::destroy_instance(m_vkbInstance);
	}

	VulkanInstance::VulkanInstance(VulkanInstance&& other) noexcept
	{
		_swap(std::move(other));
	}

	VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other) noexcept
	{
		if (this != &other)
			_swap(std::move(other));
		return *this;
	}

	void VulkanInstance::_swap(VulkanInstance&& other)
	{
		if (m_vkbInstance == VK_NULL_HANDLE || other.m_vkbInstance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (m_vkbInstance.instance != VK_NULL_HANDLE)
			vkDestroyInstance(m_vkbInstance.instance, nullptr);

		m_vkbInstance.instance = other.m_vkbInstance.instance;
		other.m_vkbInstance.instance = VK_NULL_HANDLE;
	}


	std::vector<const char*> VulkanInstance::_getRequiredExtensions()
	{
		std::uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		return {glfwExtensions, glfwExtensions + glfwExtensionCount};
	}

	VkInstance VulkanInstance::Handle() const
	{
		if (m_vkbInstance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");
		return m_vkbInstance.instance;
	}

	VulkanInstance::operator VkInstance() const
	{
		return m_vkbInstance.instance;
	}
};


