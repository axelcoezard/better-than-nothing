#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanSwapChain::VulkanSwapChain(ApplicationContext* context): m_context(context)
	{
		_createSwapchain();
		LOG_SUCCESS("Vulkan swap chain: ok");
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		_cleanupSwapchain();
	}

	void VulkanSwapChain::_createSwapchain()
	{
		auto swapChainSupport = m_context->GetVulkanDevice()->GetSwapChainSupport();

		VkSurfaceFormatKHR surfaceFormat = _chooseSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = _choosePresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = _chooseExtent(swapChainSupport.capabilities);

		uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			imageCount = swapChainSupport.capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_context->GetVulkanSurface()->Handle();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = m_context->GetVulkanDevice()->GetQueueFamilies();
		uint32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform; // No transformation

		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // No blending with other windows

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE; // Clip obscured pixels

		createInfo.oldSwapchain = VK_NULL_HANDLE; // For recreation

		auto device = m_context->GetVulkanDevice()->LogicalHandle();

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
			throw std::runtime_error("Failed to create swap chain!");

		m_imageFormat = surfaceFormat.format;
		m_extent = extent;
	}

	void VulkanSwapChain::CreateImages()
	{
		auto device = m_context->GetVulkanDevice()->LogicalHandle();

		uint32 finalImageCount = 0;
		vkGetSwapchainImagesKHR(device, m_swapChain, &finalImageCount, nullptr);

		m_images.resize(finalImageCount);
		vkGetSwapchainImagesKHR(device, m_swapChain, &finalImageCount, m_images.data());
	}

	void VulkanSwapChain::CreateImageViews()
	{
		const uint32 imageCount = m_images.size();

		m_imageViews.resize(imageCount);

		for (size_t i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_images.at(i);
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_imageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			m_imageViews.at(i) = std::make_unique<VulkanImageView>(createInfo, m_context);
		}
	}

	void VulkanSwapChain::CreateFramebuffers()
	{
		m_framebuffers.resize(m_imageViews.size());

		for (size_t i = 0; i < m_imageViews.size(); i++)
		{
			auto imageView = m_imageViews.at(i)->Handle();
			auto renderPass = m_context->GetRenderer()->GetVulkanRenderPass()->Handle();

			std::array<VkImageView, 1> attachments = { imageView };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = renderPass;
			createInfo.attachmentCount = static_cast<uint32>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_extent.width;
			createInfo.height = m_extent.height;
			createInfo.layers = 1;

			auto framebuffer = std::make_unique<VulkanFramebuffer>(&createInfo, m_context);
			m_framebuffers.at(i) = std::move(framebuffer);
		}
	}

	void VulkanSwapChain::RecreateSwapchain()
	{
		auto [width, height] = m_context->GetWindow()->GetFramebufferSize();

		while (width == 0 || height == 0)
		{
			auto framebufferSize = m_context->GetWindow()->GetFramebufferSize();
			width = framebufferSize.first;
			height = framebufferSize.second;

			glfwWaitEvents();
		}

		m_context->GetVulkanDevice()->WaitIdle();

		_cleanupSwapchain();

		_createSwapchain();
		CreateImages();
		CreateImageViews();
		CreateFramebuffers();
	}

	void VulkanSwapChain::_cleanupSwapchain()
	{
		m_framebuffers.clear();
		m_imageViews.clear();

		vkDestroySwapchainKHR(m_context->GetVulkanDevice()->LogicalHandle(), m_swapChain, nullptr);
	}

	VkSurfaceFormatKHR VulkanSwapChain::_chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& surfaceFormat : availableFormats)
		{
			const bool correctFormat = surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB;
			const bool correctColorSpace = surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

			if (correctFormat && correctColorSpace)
				return surfaceFormat;
		}
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapChain::_choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) // TripleBuffering buffering
				return availablePresentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR; // V-Sync
	}

	VkExtent2D VulkanSwapChain::_chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32>::max())
			return capabilities.currentExtent;

		auto [width, height] = m_context->GetWindow()->GetFramebufferSize();

		VkExtent2D actualExtent;
		actualExtent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	VkFormat VulkanSwapChain::GetImageFormat() const
	{
		return m_imageFormat;
	}

	uint32 VulkanSwapChain::GetImageCount() const
	{
		return m_images.size();
	}

	VkExtent2D VulkanSwapChain::GetExtent() const
	{
		return m_extent;
	}

	VkFramebuffer VulkanSwapChain::GetFramebuffer(uint32 imageIndex)
	{
		if (imageIndex >= m_framebuffers.size())
			throw std::runtime_error("Framebuffer index out of range");
		return m_framebuffers.at(imageIndex)->Handle();
	}

	VkSwapchainKHR VulkanSwapChain::Handle() const
	{
		return m_swapChain;
	}
};
