//
// Created by acoezard on 11/5/24.
//

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace BetterThanNothing
{
	class VulkanInstance
	{
		VkInstance m_instance = VK_NULL_HANDLE;

	public:
		explicit VulkanInstance(bool enableValidationLayers = false);
		~VulkanInstance();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		VulkanInstance(VulkanInstance&& other) noexcept;
		VulkanInstance& operator=(VulkanInstance&& other) noexcept;

		VkInstance Handle() const;
		explicit operator VkInstance() const;

	private:
		void _move(VulkanInstance&& other) noexcept;
	};
};


