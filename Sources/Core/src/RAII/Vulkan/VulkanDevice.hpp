//
// Created by acoezard on 11/6/24.
//

#pragma once

#include <optional>

namespace BetterThanNothing
{
	class ApplicationContext;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() const {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VulkanDevice
	{
		ApplicationContext* m_context = nullptr;

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
		VulkanDevice(VulkanDevice&& other) = delete;
		VulkanDevice& operator=(VulkanDevice&& other) = delete;

		[[nodiscard]]
		VkPhysicalDevice Handle() const;

		[[nodiscard]]
		std::string GetVendorName() const;

		[[nodiscard]]
		std::string GetDeviceName() const;

		[[nodiscard]]
		std::string GetApiVersion() const;

	private:
		bool _isDeviceSuitable(VkPhysicalDevice device) noexcept;
		VkPhysicalDevice _findSuitableDevice(const std::vector<VkPhysicalDevice>& devices);
		QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);

		[[nodiscard]]
		std::string _getVendorById(uint32_t vendorId) const;

		void _pickPhysicalDevice();
		void _getPhysicalDeviceProperties();
		void _createLogicalDevice(ApplicationContext* context);
	};
};
