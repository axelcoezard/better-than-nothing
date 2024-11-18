#pragma once


namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanSwapchain {
	private:
		ApplicationContext* m_context = nullptr;
		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	public:
		explicit VulkanSwapchain(ApplicationContext* context);
		~VulkanSwapchain();

		VulkanSwapchain(const VulkanSwapchain&) = delete;
		VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
		VulkanSwapchain(VulkanSwapchain&& other) = delete;
		VulkanSwapchain& operator=(VulkanSwapchain&& other) = delete;

	private:
		[[nodiscard]]
		VkSurfaceFormatKHR _chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		[[nodiscard]]
		VkPresentModeKHR _choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		[[nodiscard]]
		VkExtent2D _chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	};
};
