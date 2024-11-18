#include "VulkanSwapChain.hpp"
#include "ApplicationContext.hpp"

namespace BetterThanNothing
{
	VulkanSwapChain::VulkanSwapChain(ApplicationContext* context): m_context(context)
	{
		auto swapChainSupport = m_context->GetVulkanDevice()->GetSwapChainSupport();

		VkSurfaceFormatKHR surfaceFormat = _chooseSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = _choosePresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = _chooseExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

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
		uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

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

		if (m_context->GetVulkanDevice()->CreateSwapChain(&createInfo, &m_swapChain) != VK_SUCCESS)
			throw std::runtime_error("Failed to create swap chain!");

		LOG_SUCCESS("Vulkan swap chain: ok");
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		m_context->GetVulkanDevice()->DestroySwapChain(m_swapChain);
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
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) // Triple buffering
				return availablePresentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR; // V-Sync
	}

	VkExtent2D VulkanSwapChain::_chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		auto [width, height] = m_context->GetWindow()->GetFramebufferSize();

		VkExtent2D actualExtent;
		actualExtent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
};
