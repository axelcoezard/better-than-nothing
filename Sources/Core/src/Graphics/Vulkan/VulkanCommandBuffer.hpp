#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanCommandBuffer
	{
	private:
		VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;

		bool m_bRecording = false;

	public:
		explicit VulkanCommandBuffer(VkCommandBuffer commandBuffer);
		~VulkanCommandBuffer() = default;

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
