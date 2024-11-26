#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanFramebuffer
	{
	private:
		ApplicationContext* m_context;
		VkFramebuffer m_framebuffer = VK_NULL_HANDLE;

	public:
		explicit VulkanFramebuffer(VkFramebufferCreateInfo* createInfo, ApplicationContext* context);
		~VulkanFramebuffer();

		VulkanFramebuffer(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer(VulkanFramebuffer&& other) = delete;
		VulkanFramebuffer& operator=(VulkanFramebuffer&& other) = delete;

		[[nodiscard]]
		VkFramebuffer Handle() const;
	};
}
