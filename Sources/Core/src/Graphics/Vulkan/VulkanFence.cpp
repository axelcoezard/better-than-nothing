#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanFence::VulkanFence(ApplicationContext* context): m_context(context)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

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

	void VulkanFence::Wait()
	{
		auto device = m_context->GetVulkanDevice()->LogicalHandle();
		vkWaitForFences(device, 1, &m_fence, VK_TRUE, UINT64_MAX);
	}

	void VulkanFence::Reset()
	{
		auto device = m_context->GetVulkanDevice()->LogicalHandle();
		vkResetFences(device, 1, &m_fence);
	}

	VkFence VulkanFence::Handle() const
	{
		return m_fence;
	}
}