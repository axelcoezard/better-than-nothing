#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanFence::VulkanFence(ApplicationContext* context): m_context(context)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = 0;

		auto device = m_context->GetVulkanDevice()->LogicalHandle();
		if (vkCreateFence(device, &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
			throw std::runtime_error("failed to create fence!");
	}

	VulkanFence::~VulkanFence()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
			auto device = m_context->GetVulkanDevice()->LogicalHandle();
			vkDestroyFence(device, m_fence, nullptr);
		}
	}

	VkFence VulkanFence::Handle() const
	{
		return m_fence;
	}
}