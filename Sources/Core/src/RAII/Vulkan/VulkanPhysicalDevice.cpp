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

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

		m_vendorName = _getVendorById(deviceProperties.vendorID);
		m_deviceName = deviceProperties.deviceName;
		m_apiVersion = std::to_string(deviceProperties.apiVersion);

		LOG_INFO("Vulkan physical device: ok");
		LOG("-> Vendor: " << m_vendorName);
		LOG("-> Device: " << m_deviceName);
		LOG("-> API Version: " << m_apiVersion);
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

	std::string VulkanPhysicalDevice::_getVendorById(const uint32_t vendorId) const
	{
		if (vendorId == 0x1002) return "AMD";
		if (vendorId == 0x1010) return "ImgTec";
		if (vendorId == 0x10DE) return "NVIDIA";
		if (vendorId == 0x13B5) return "ARM";
		if (vendorId == 0x5143) return "Qualcomm";
		if (vendorId == 0x8086) return "INTEL";
		return "Unknown";
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

	std::string VulkanPhysicalDevice::GetVendorName() const
	{
		return m_vendorName;
	}

	std::string VulkanPhysicalDevice::GetDeviceName() const
	{
		return m_deviceName;
	}

	std::string VulkanPhysicalDevice::GetApiVersion() const
	{
		return m_apiVersion;
	}
}
