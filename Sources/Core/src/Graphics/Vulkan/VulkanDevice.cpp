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

		const auto deviceExtensions = context->GetDeviceExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

	VkPhysicalDevice VulkanDevice::_findSuitableDevice(const std::vector<VkPhysicalDevice>& devices)
	{
		for (const auto& device : devices)
		{
			if (_isDeviceSuitable(device))
				return device;
		}
		throw std::runtime_error("Failed to find a suitable GPU");
	}

	bool VulkanDevice::_isDeviceSuitable(VkPhysicalDevice device) noexcept
	{
		QueueFamilyIndices queueFamilyIndices = _findQueueFamilies(device);

		bool queueFamiliesSupported = queueFamilyIndices.IsComplete();
		bool extensionsSupported = _checkDeviceExtensionSupport(device);
		bool swapchainSupported = false;

		if (extensionsSupported)
		{
			auto swapChainSupport = _querySwapChainSupport(device);
			swapchainSupported = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return queueFamiliesSupported && extensionsSupported && swapchainSupported;
	}

	bool VulkanDevice::_checkDeviceExtensionSupport(VkPhysicalDevice device) noexcept
	{
		auto deviceExtensions = m_context->GetDeviceExtensions();

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanDevice::_findQueueFamilies(VkPhysicalDevice device) const
	{
		auto vkSurface = m_context->GetVulkanSurface()->Handle();

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		QueueFamilyIndices indices;

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

	QueueFamilyIndices VulkanDevice::GetQueueFamilies() const
	{
		return _findQueueFamilies(m_physicalDevice);
	}

	SwapChainSupportDetails VulkanDevice::_querySwapChainSupport(VkPhysicalDevice device) const
	{
		auto surface = m_context->GetVulkanSurface()->Handle();

		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	SwapChainSupportDetails VulkanDevice::GetSwapChainSupport() const
	{
		return _querySwapChainSupport(m_physicalDevice);
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

	VkPhysicalDevice VulkanDevice::PhysicalHandle() const
	{
		if (m_physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan physical device is not initialized");
		return m_physicalDevice;
	}

	VkDevice VulkanDevice::LogicalHandle() const
	{
		if (m_logicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan logical device is not initialized");
		return m_logicalDevice;
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
