#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanFramebuffer::VulkanFramebuffer(VkFramebufferCreateInfo* createInfo, ApplicationContext* context)
		: m_context(context)
	{
		auto device = context->GetVulkanDevice()->LogicalHandle();

		if (vkCreateFramebuffer(device, createInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer!");
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyFramebuffer(m_context->GetVulkanDevice()->LogicalHandle(), m_framebuffer, nullptr);
	}

	VkFramebuffer VulkanFramebuffer::Handle() const
	{
		return m_framebuffer;
	}
}