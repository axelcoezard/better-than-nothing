#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanImageView::VulkanImageView(VkImageViewCreateInfo& createInfo, ApplicationContext* context): context(context)
	{
		if (vkCreateImageView(context->GetVulkanDevice()->LogicalHandle(), &createInfo, nullptr, &m_imageView) != VK_SUCCESS)
			throw std::runtime_error("Failed to create image view");
	}

	VulkanImageView::~VulkanImageView()
	{
		vkDestroyImageView(context->GetVulkanDevice()->LogicalHandle(), m_imageView, nullptr);
	}
};