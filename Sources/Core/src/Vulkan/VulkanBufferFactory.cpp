//
// Created by acoezard on 11/25/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanBuffer VulkanBufferFactory::CreateVertexBuffer(const void* data, const uint32 dataSize) const
	{
		VulkanBuffer stagingBuffer = {dataSize, VulkanBufferType::STAGING, m_context};
		VulkanBuffer vertexBuffer = {dataSize, VulkanBufferType::VERTEX, m_context};

		void* stagingData;
		stagingBuffer.MapMemory(&stagingData);
		memcpy(stagingData, data, dataSize);
		stagingBuffer.UnmapMemory();

		VulkanCommandBuffer::SingleTimeCommands([&](const std::unique_ptr<VulkanCommandBuffer>& commandBuffer) {
			commandBuffer->CmdCopyBuffer(stagingBuffer.Handle(), vertexBuffer.Handle(), dataSize);
		}, m_context);

		return vertexBuffer;
	}

	VulkanBuffer VulkanBufferFactory::CreateIndexBuffer(const void* data, const uint32 dataSize) const
	{
		VulkanBuffer stagingBuffer = {dataSize, VulkanBufferType::STAGING, m_context};
		VulkanBuffer indexBuffer = {dataSize, VulkanBufferType::INDEX, m_context};

		void* stagingData;
		stagingBuffer.MapMemory(&stagingData);
		memcpy(stagingData, data, dataSize);
		stagingBuffer.UnmapMemory();

		VulkanCommandBuffer::SingleTimeCommands([&](const std::unique_ptr<VulkanCommandBuffer>& commandBuffer) {
			commandBuffer->CmdCopyBuffer(stagingBuffer.Handle(), indexBuffer.Handle(), dataSize);
		}, m_context);

		return indexBuffer;
	}
}