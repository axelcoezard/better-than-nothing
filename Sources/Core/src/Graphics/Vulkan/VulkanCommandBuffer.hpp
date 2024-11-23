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

		void CmdBeginRenderPass(VkRenderPassBeginInfo* pRenderPassInfo, VkSubpassContents subpassContents = VK_SUBPASS_CONTENTS_INLINE);
		void CmdEndRenderPass();

		void CmdBindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		void CmdSetViewport(const VkViewport* pViewports, uint32_t firstViewport = 0, uint32_t viewportCount = 1);
		void CmdSetScissor(const VkRect2D* pScissors, uint32_t firstScissor = 0, uint32_t scissorCount = 1);
		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

		[[nodiscard]]
		VkCommandBuffer Handle() const;
	};

}
