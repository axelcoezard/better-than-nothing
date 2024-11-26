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

	void VulkanCommandBuffer::BeginRecording(const VkCommandBufferUsageFlags flags)
	{
		if (m_bRecording)
			throw std::runtime_error("Command buffer is already recording");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = flags;

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

	void VulkanCommandBuffer::CmdBeginRenderPass(VkRenderPassBeginInfo* renderPassInfo, VkSubpassContents subpassContents) const
	{
		vkCmdBeginRenderPass(m_commandBuffer, renderPassInfo, subpassContents);
	}

	void VulkanCommandBuffer::CmdEndRenderPass() const
	{
		vkCmdEndRenderPass(m_commandBuffer);
	}

	void VulkanCommandBuffer::CmdSetViewport(const VkViewport* viewports, uint32_t firstViewport, uint32_t viewportCount) const
	{
		vkCmdSetViewport(m_commandBuffer, firstViewport, viewportCount, viewports);
	}

	void VulkanCommandBuffer::CmdSetScissor(const VkRect2D* scissors, uint32_t firstScissor, uint32_t scissorCount) const
	{
		vkCmdSetScissor(m_commandBuffer, firstScissor, scissorCount, scissors);
	}

	void VulkanCommandBuffer::CmdBindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint) const
	{
		vkCmdBindPipeline(m_commandBuffer, bindPoint, pipeline);
	}

	void VulkanCommandBuffer::CmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* buffers, const VkDeviceSize* offsets) const
	{
		vkCmdBindVertexBuffers(m_commandBuffer, firstBinding, bindingCount, buffers, offsets);
	}

	void VulkanCommandBuffer::CmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const
	{
		vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
	}

	void VulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const
	{
		vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
	{
		vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void VulkanCommandBuffer::CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const uint32_t bufferSize) const
	{
		VkBufferCopy copyRegion{};
		copyRegion.size = bufferSize;

		vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	}

	void VulkanCommandBuffer::SingleTimeCommands(const std::function<void(std::unique_ptr<VulkanCommandBuffer>&)>& callback, ApplicationContext* context)
	{
		const auto device = context->GetVulkanDevice()->LogicalHandle();
		const auto commandPool = context->GetRenderer()->GetVulkanCommandPool()->Handle();
		const auto graphicsQueue = context->GetGraphicsQueue()->Handle();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		auto pCommandBuffer = std::make_unique<VulkanCommandBuffer>(commandBuffer);

		pCommandBuffer->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		callback(pCommandBuffer);
		pCommandBuffer->EndRecording();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}


	VkCommandBuffer VulkanCommandBuffer::Handle() const
	{
		return m_commandBuffer;
	}
}