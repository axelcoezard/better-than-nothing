#pragma once


namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanImageView
	{
	private:
		ApplicationContext* context = nullptr;
		VkImageView m_imageView = VK_NULL_HANDLE;

	public:
		explicit VulkanImageView(VkImageViewCreateInfo& createInfo, ApplicationContext* context);
		~VulkanImageView();

		VulkanImageView(const VulkanImageView&) = delete;
		VulkanImageView& operator=(const VulkanImageView&) = delete;
		VulkanImageView(VulkanImageView&& other) = delete;
		VulkanImageView& operator=(VulkanImageView&& other) = delete;

		[[nodiscard]]
		VkImageView Handle() const { return m_imageView; }
	};
};
