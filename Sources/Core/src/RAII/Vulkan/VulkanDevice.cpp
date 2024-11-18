//
// Created by acoezard on 11/6/24.
//

#include "VulkanDevice.hpp"
#include "ApplicationContext.hpp"

namespace BetterThanNothing
{
	VulkanDevice::VulkanDevice(ApplicationContext* context): m_context(context)
	{
		_pickPhysicalDevice();
		_getPhysicalDeviceProperties();
		_createLogicalDevice(context);
	}

	VulkanDevice::~VulkanDevice()
	{
		if (m_logicalDevice != VK_NULL_HANDLE)
			vkDestroyDevice(m_logicalDevice, nullptr);
	}

	void VulkanDevice::_pickPhysicalDevice()
	{
		auto vkInstance = m_context->GetVulkanInstance()->Handle();

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

		m_physicalDevice = _findSuitableDevice(devices);

		LOG_SUCCESS("Vulkan physical device: ok");
	}

	void VulkanDevice::_getPhysicalDeviceProperties()
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

		m_vendorName = _getVendorById(deviceProperties.vendorID);
		m_deviceName = deviceProperties.deviceName;
		m_apiVersion = std::to_string(deviceProperties.apiVersion);

		LOG_INFO("Vendor: " << m_vendorName);
		LOG_INFO("Device: " << m_deviceName);
		LOG_INFO("API Version: " << m_apiVersion);
	}

	void VulkanDevice::_createLogicalDevice(ApplicationContext* context)
	{
		QueueFamilyIndices indices = _findQueueFamilies(m_physicalDevice);
		if (!indices.IsComplete())
			throw std::runtime_error("Failed to find queue families");

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set uniqueQueueFamilies = {
			indices.graphicsFamily.value(),
			indices.presentFamily.value()
		};

		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;

		if (context->IsValidationLayersEnabled()) {
			const auto validationLayers = context->GetValidationLayers();

			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
			throw std::runtime_error("Failed to create logical device");

		LOG_SUCCESS("Vulkan logical device: ok");

		VkQueue graphicsQueue;
		vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
		m_context->SetGraphicsQueue(graphicsQueue);

		LOG_SUCCESS("Vulkan graphics queue: ok");

		VkQueue presentQueue;
		vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
		m_context->SetPresentQueue(presentQueue);

		LOG_SUCCESS("Vulkan presentation queue: ok");
	}

	bool VulkanDevice::_isDeviceSuitable(VkPhysicalDevice device) noexcept
	{
		QueueFamilyIndices indices = _findQueueFamilies(device);
		return indices.IsComplete();
	}

	VkPhysicalDevice VulkanDevice::_findSuitableDevice(const std::vector<VkPhysicalDevice>& devices)
	{
		for (const auto& device : devices)
		{
			if (_isDeviceSuitable(device))
				return device;
		}
		throw std::runtime_error("Failed to find a suitable GPU");
	}

	std::string VulkanDevice::_getVendorById(const uint32_t vendorId) const
	{
		if (vendorId == 0x1002) return "AMD";
		if (vendorId == 0x1010) return "ImgTec";
		if (vendorId == 0x10DE) return "NVIDIA";
		if (vendorId == 0x13B5) return "ARM";
		if (vendorId == 0x5143) return "Qualcomm";
		if (vendorId == 0x8086) return "INTEL";
		return "Unknown";
	}

	QueueFamilyIndices VulkanDevice::_findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0)
			throw std::runtime_error("Failed to find queue families");

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		auto vkSurface = m_context->GetVulkanSurface()->Handle();

		uint32_t i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			// Check if the queue family supports graphics queue
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			// Check if the queue family supports present queue
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkSurface, &presentSupport);
			if (presentSupport)
				indices.presentFamily = i;

			// If both graphics and present queues are supported, then we are done
			if (indices.IsComplete())
				break;
			i++;
		}

		return indices;
	}

	VkPhysicalDevice VulkanDevice::Handle() const
	{
		if (m_physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan physical device is not initialized");
		return m_physicalDevice;
	}

	std::string VulkanDevice::GetVendorName() const
	{
		return m_vendorName;
	}

	std::string VulkanDevice::GetDeviceName() const
	{
		return m_deviceName;
	}

	std::string VulkanDevice::GetApiVersion() const
	{
		return m_apiVersion;
	}
}
