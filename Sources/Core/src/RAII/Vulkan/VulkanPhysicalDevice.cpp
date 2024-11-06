//
// Created by acoezard on 11/6/24.
//

#include "VulkanPhysicalDevice.hpp"
#include "ApplicationContext.hpp"

namespace BetterThanNothing
{
	VulkanPhysicalDevice::VulkanPhysicalDevice(const ApplicationContext* context)
		: m_instance(context->vulkanInstance.Handle())
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		m_physicalDevice = _findSuitableDevice(devices);

		LOG_INFO("Vulkan physical device: ok ");
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanPhysicalDevice&& other) noexcept
	{
		_move(std::move(other));
	}

	VulkanPhysicalDevice& VulkanPhysicalDevice::operator=(VulkanPhysicalDevice&& other) noexcept
	{
		if (this != &other)
			_move(std::move(other));
		return *this;
	}

	void VulkanPhysicalDevice::_move(VulkanPhysicalDevice&& other) noexcept
	{
		// TODO: if m_physicalDevice != VK_NULL_HANDLE then destroy

		m_physicalDevice = other.m_physicalDevice;
		other.m_physicalDevice = VK_NULL_HANDLE;
	}

	bool VulkanPhysicalDevice::_isDeviceSuitable(VkPhysicalDevice device) noexcept
	{
		(void) device;
		return true;
	}

	VkPhysicalDevice VulkanPhysicalDevice::_findSuitableDevice(const std::vector<VkPhysicalDevice>& devices)
	{
		for (const auto& device : devices)
		{
			if (_isDeviceSuitable(device))
				return device;
		}
		throw std::runtime_error("Failed to find a suitable GPU");
	}

	VkPhysicalDevice VulkanPhysicalDevice::Handle() const
	{
		if (m_physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan physical device is not initialized");
		return m_physicalDevice;
	}

	VulkanPhysicalDevice::operator VkPhysicalDevice() const
	{
		return m_physicalDevice;
	}
}
