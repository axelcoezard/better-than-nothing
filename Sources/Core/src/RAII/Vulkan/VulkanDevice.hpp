//
// Created by acoezard on 11/6/24.
//

#pragma once

#include <optional>

namespace BetterThanNothing
{
	struct ApplicationContext;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete() {
			return graphicsFamily.has_value();
		}
	};

	class VulkanDevice
	{
		VkInstance m_instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_logicalDevice = VK_NULL_HANDLE;

		std::string m_vendorName;
		std::string m_deviceName;
		std::string m_apiVersion;
	public:
		VulkanDevice() = default;
		explicit VulkanDevice(ApplicationContext* context);
		~VulkanDevice();

		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(const VulkanDevice&) = delete;

		VulkanDevice(VulkanDevice&& other) noexcept;
		VulkanDevice& operator=(VulkanDevice&& other) noexcept;

		VkPhysicalDevice Handle() const;
		explicit operator VkPhysicalDevice() const;

		std::string GetVendorName() const;
		std::string GetDeviceName() const;
		std::string GetApiVersion() const;

	private:
		void _move(VulkanDevice&& other) noexcept;
		bool _isDeviceSuitable(VkPhysicalDevice device) noexcept;
		VkPhysicalDevice _findSuitableDevice(const std::vector<VkPhysicalDevice>& devices);
		std::string _getVendorById(uint32_t vendorId) const;
		QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);

		void _pickPhysicalDevice();
		void _getPhysicalDeviceProperties();
		void _createLogicalDevice(ApplicationContext* context);
	};
};
