#include "VulkanImageView.hpp"
#include "ApplicationContext.hpp"

namespace BetterThanNothing
{
	VulkanImageView::VulkanImageView(VkImageViewCreateInfo& createInfo, ApplicationContext* context): context(context)
	{
		if (vkCreateImageView(context->GetVulkanDevice()->Handle(), &createInfo, nullptr, &m_imageView) != VK_SUCCESS)
			throw std::runtime_error("Failed to create image view");
	}

	VulkanImageView::~VulkanImageView()
	{
		vkDestroyImageView(context->GetVulkanDevice()->Handle(), m_imageView, nullptr);
	}
};