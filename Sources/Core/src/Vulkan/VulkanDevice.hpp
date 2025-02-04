//
// Created by acoezard on 11/6/24.
//

#pragma once

#include <optional>

namespace BetterThanNothing
{
	class ApplicationContext;

	struct QueueFamilyIndices
	{
		std::optional<uint32> graphicsFamily;
		std::optional<uint32> presentFamily;

		bool IsComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanDevice
	{
	private:
		ApplicationContext* m_context = nullptr;

		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_logicalDevice = VK_NULL_HANDLE;

		std::string m_vendorName;
		std::string m_deviceName;
		std::string m_apiVersion;
	public:
		explicit VulkanDevice(ApplicationContext* context);
		~VulkanDevice();

		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&& other) = delete;
		VulkanDevice& operator=(VulkanDevice&& other) = delete;

		[[nodiscard]]
		VkPhysicalDevice PhysicalHandle() const;

		[[nodiscard]]
		VkDevice LogicalHandle() const;

		[[nodiscard]]
		std::string GetVendorName() const;

		[[nodiscard]]
		std::string GetDeviceName() const;

		[[nodiscard]]
		std::string GetApiVersion() const;

		[[nodiscard]]
		SwapChainSupportDetails GetSwapChainSupport() const;

		[[nodiscard]]
		QueueFamilyIndices GetQueueFamilies() const;

		void WaitIdle() const;

	private:
		VkPhysicalDevice _findSuitableDevice(const std::vector<VkPhysicalDevice>& devices);
		bool _isDeviceSuitable(VkPhysicalDevice device) noexcept;
		bool _checkDeviceExtensionSupport(VkPhysicalDevice device) noexcept;
		QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device) const;
		SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device) const;

		[[nodiscard]]
		std::string _getVendorById(uint32 vendorId) const;

		void _pickPhysicalDevice();
		void _getPhysicalDeviceProperties();
		void _createLogicalDevice(ApplicationContext* context);
	};
};
