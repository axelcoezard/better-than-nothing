//
// Created by acoezard on 11/6/24.
//

#pragma once

namespace BetterThanNothing
{
	struct ApplicationContext;

	class VulkanPhysicalDevice
	{
		VkInstance m_instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

		std::string m_vendorName;
		std::string m_deviceName;
		std::string m_apiVersion;
	public:
		VulkanPhysicalDevice() = default;
		explicit VulkanPhysicalDevice(const ApplicationContext* context);
		~VulkanPhysicalDevice();

		VulkanPhysicalDevice(const VulkanPhysicalDevice&) = delete;
		VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice&) = delete;

		VulkanPhysicalDevice(VulkanPhysicalDevice&& other) noexcept;
		VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&& other) noexcept;

		VkPhysicalDevice Handle() const;
		explicit operator VkPhysicalDevice() const;

		std::string GetVendorName() const;
		std::string GetDeviceName() const;
		std::string GetApiVersion() const;

	private:
		void _move(VulkanPhysicalDevice&& other) noexcept;
		bool _isDeviceSuitable(VkPhysicalDevice device) noexcept;
		VkPhysicalDevice _findSuitableDevice(const std::vector<VkPhysicalDevice>& devices);
		std::string _getVendorById(const uint32_t vendorId) const;
	};
};
