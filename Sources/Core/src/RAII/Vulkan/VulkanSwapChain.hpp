#pragma once


namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanSwapChain {
	private:
		ApplicationContext* m_context = nullptr;
		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	public:
		explicit VulkanSwapChain(ApplicationContext* context);
		~VulkanSwapChain();

		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
		VulkanSwapChain(VulkanSwapChain&& other) = delete;
		VulkanSwapChain& operator=(VulkanSwapChain&& other) = delete;

	private:
		[[nodiscard]]
		VkSurfaceFormatKHR _chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		[[nodiscard]]
		VkPresentModeKHR _choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		[[nodiscard]]
		VkExtent2D _chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	};
};
