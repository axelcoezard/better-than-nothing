#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanCommandBuffer
	{
	private:
		ApplicationContext* m_context = nullptr;

		VkCommandPool m_commandPool = VK_NULL_HANDLE;
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;

		bool m_bRecording = false;

	public:
		explicit VulkanCommandBuffer(VkCommandPool commandPool, ApplicationContext* context);
		~VulkanCommandBuffer();

		VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer(VulkanCommandBuffer&& other) = delete;
		VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) = delete;

		void BeginRecording();
		void EndRecording();

		[[nodiscard]]
		VkCommandBuffer Handle() const;
	};

}
