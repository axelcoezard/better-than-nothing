//
// Created by acoezard on 11/25/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanBuffer::VulkanBuffer(uint32_t size, VkBufferUsageFlagBits usage, ApplicationContext* context): m_context(context), m_size(size)
	{
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (m_context->GetVulkanMemoryAllocator()->CreateBuffer(&bufferCreateInfo, &m_buffer, &m_allocation, &m_allocationInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan buffer");
	}

	VulkanBuffer::~VulkanBuffer()
	{
		m_context->GetVulkanMemoryAllocator()->DestroyBuffer(m_buffer, m_allocation);
	}

	VkResult VulkanBuffer::MapMemory(void** data) const
	{
		return m_context->GetVulkanMemoryAllocator()->MapMemory(m_allocation, data);
	}

	void VulkanBuffer::UnmapMemory() const
	{
		return m_context->GetVulkanMemoryAllocator()->UnmapMemory(m_allocation);
	}

	uint32_t VulkanBuffer::Size() const
	{
		return m_size;
	}

	VkBuffer VulkanBuffer::Handle() const
	{
		return m_buffer;
	}
}