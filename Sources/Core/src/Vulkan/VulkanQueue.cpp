//
// Created by acoezard on 11/18/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanQueue::VulkanQueue(VkQueue queue): m_queue(queue) {}

	VkQueue VulkanQueue::Handle() const
	{
		return m_queue;
	}
}