#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanCommandPool
	{
	private:
		ApplicationContext* m_context = nullptr;
		VkCommandPool m_commandPool = VK_NULL_HANDLE;

	public:
		explicit VulkanCommandPool(ApplicationContext* context);
		~VulkanCommandPool();

		VulkanCommandPool(const VulkanCommandPool&) = delete;
		VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
		VulkanCommandPool(VulkanCommandPool&& other) = delete;
		VulkanCommandPool& operator=(VulkanCommandPool&& other) = delete;

		[[nodiscard]]
		VkCommandPool Handle() const;
	};
}
