#include "ApplicationContext.hpp"
#include "VulkanCommandBuffer.hpp"

namespace BetterThanNothing
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer)
		: m_commandBuffer(commandBuffer)
	{
	}

	void VulkanCommandBuffer::Reset()
	{
		if (m_bRecording)
			throw std::runtime_error("Command buffer is recording");

		if (vkResetCommandBuffer(m_commandBuffer, 0) != VK_SUCCESS)
			throw std::runtime_error("failed to reset command buffer!");
	}

	void VulkanCommandBuffer::BeginRecording()
	{
		if (m_bRecording)
			throw std::runtime_error("Command buffer is already recording");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		m_bRecording = true;
	}

	void VulkanCommandBuffer::EndRecording()
	{
		if (!m_bRecording)
			throw std::runtime_error("Command buffer is not recording");

		if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");

		m_bRecording = false;
	}

	void VulkanCommandBuffer::CmdBeginRenderPass(VkRenderPassBeginInfo* renderPassInfo, VkSubpassContents subpassContents)
	{
		vkCmdBeginRenderPass(m_commandBuffer, renderPassInfo, subpassContents);
	}

	void VulkanCommandBuffer::CmdEndRenderPass()
	{
		vkCmdEndRenderPass(m_commandBuffer);
	}

	void VulkanCommandBuffer::CmdBindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint)
	{
		vkCmdBindPipeline(m_commandBuffer, bindPoint, pipeline);
	}

	void VulkanCommandBuffer::CmdSetViewport(const VkViewport* viewports, uint32_t firstViewport, uint32_t viewportCount)
	{
		vkCmdSetViewport(m_commandBuffer, firstViewport, viewportCount, viewports);
	}

	void VulkanCommandBuffer::CmdSetScissor(const VkRect2D* scissors, uint32_t firstScissor, uint32_t scissorCount)
	{
		vkCmdSetScissor(m_commandBuffer, firstScissor, scissorCount, scissors);
	}

	void VulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	VkCommandBuffer VulkanCommandBuffer::Handle() const
	{
		return m_commandBuffer;
	}
}