//
// Created by acoezard on 11/25/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanBuffer VulkanBufferFactory::CreateVertexBuffer(const void* data, const uint32_t dataSize) const
	{
		VulkanBuffer stagingBuffer = {dataSize, VulkanBufferType::STAGING_BUFFER, m_context};
		VulkanBuffer vertexBuffer = {dataSize, VulkanBufferType::VERTEX_BUFFER, m_context};

		void* stagingData;
		stagingBuffer.MapMemory(&stagingData);
		memcpy(stagingData, data, dataSize);
		stagingBuffer.UnmapMemory();

		VulkanCommandBuffer::SingleTimeCommands([&](const std::unique_ptr<VulkanCommandBuffer>& commandBuffer) {
			commandBuffer->CmdCopyBuffer(stagingBuffer.Handle(), vertexBuffer.Handle(), dataSize);
		}, m_context);

		return vertexBuffer;
	}
}