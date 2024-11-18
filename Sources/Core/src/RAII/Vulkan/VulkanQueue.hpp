//
// Created by acoezard on 11/18/24.
//

#pragma once

namespace BetterThanNothing
{
	class VulkanQueue
	{
	private:
		VkQueue m_queue = VK_NULL_HANDLE;

	public:
		explicit VulkanQueue(VkQueue queue);
		~VulkanQueue() = default;

		VulkanQueue(const VulkanQueue&) = delete;
		VulkanQueue& operator=(const VulkanQueue&) = delete;
		VulkanQueue(VulkanQueue&& other) = delete;
		VulkanQueue& operator=(VulkanQueue&& other) = delete;

		[[nodiscard]]
		VkQueue Handle() const;
	};
} // BetterThanNothing