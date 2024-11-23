#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanSemaphore::VulkanSemaphore(ApplicationContext* context): m_context(context)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = 0;

		auto device = m_context->GetVulkanDevice()->LogicalHandle();
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore!");
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			auto device = m_context->GetVulkanDevice()->LogicalHandle();
			vkDestroySemaphore(device, m_semaphore, nullptr);
		}
	}

	VkSemaphore VulkanSemaphore::Handle() const
	{
		return m_semaphore;
	}
}