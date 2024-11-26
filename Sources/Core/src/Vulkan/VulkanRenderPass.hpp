#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanRenderPass
	{
	private:
		ApplicationContext* m_context = nullptr;
		VkRenderPass m_renderPass = VK_NULL_HANDLE;

	public:
		explicit VulkanRenderPass(ApplicationContext* context);
		~VulkanRenderPass();

		VulkanRenderPass(const VulkanRenderPass&) = delete;
		VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
		VulkanRenderPass(VulkanRenderPass&& other) = delete;
		VulkanRenderPass& operator=(VulkanRenderPass&& other) = delete;

		[[nodiscard]]
		VkRenderPass Handle() const;
	};
}