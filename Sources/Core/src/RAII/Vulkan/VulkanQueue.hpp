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

		// Prevent copy to ensure unique ownership of nodes
		VulkanQueue(const VulkanQueue&) = delete;
		VulkanQueue& operator=(const VulkanQueue&) = delete;

		VkQueue Handle() const;
		explicit operator VkQueue() const;
	};
} // BetterThanNothing