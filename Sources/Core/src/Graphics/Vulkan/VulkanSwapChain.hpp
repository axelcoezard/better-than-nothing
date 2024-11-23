#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;
	class VulkanImageView;
	class VulkanFramebuffer;

	class VulkanSwapChain {
	private:
		ApplicationContext* m_context = nullptr;

		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		VkFormat m_imageFormat;
		VkExtent2D m_extent;

		std::vector<VkImage> m_images;
		std::vector<std::unique_ptr<VulkanImageView>> m_imageViews;
		std::vector<std::unique_ptr<VulkanFramebuffer>> m_framebuffers;

	public:
		explicit VulkanSwapChain(ApplicationContext* context);
		~VulkanSwapChain();

		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
		VulkanSwapChain(VulkanSwapChain&& other) = delete;
		VulkanSwapChain& operator=(VulkanSwapChain&& other) = delete;

		void CreateImages();
		void CreateImageViews();
		void CreateFramebuffers();

	private:
		[[nodiscard]]
		VkSurfaceFormatKHR _chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		[[nodiscard]]
		VkPresentModeKHR _choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		[[nodiscard]]
		VkExtent2D _chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	public:
		[[nodiscard]]
		VkFormat GetImageFormat() const;

		[[nodiscard]]
		uint32_t GetImageCount() const;

		[[nodiscard]]
		VkExtent2D GetExtent() const;

		[[nodiscard]]
		VkFramebuffer GetFramebuffer(uint32_t index);
	};
};
