//
// Created by acoezard on 11/25/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanBuffer::VulkanBuffer(const uint32 size, const VulkanBufferType type, ApplicationContext* context): m_context(context), m_size(size)
	{
		const VkBufferUsageFlags usage = _getBufferUsageByBufferType(type);
		const VkMemoryPropertyFlags properties = _getMemoryPropertiesByBufferType(type);

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (m_context->GetVulkanMemoryAllocator()->CreateBuffer(&bufferCreateInfo, properties, &m_buffer, &m_allocation, &m_allocationInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan buffer");
	}

	VulkanBuffer::~VulkanBuffer()
	{
		if (m_buffer != VK_NULL_HANDLE && m_allocation != VK_NULL_HANDLE)
			m_context->GetVulkanMemoryAllocator()->DestroyBuffer(m_buffer, m_allocation);
	}

	VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept
	{
		_move(std::move(other));
	}

	VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
	{
		_move(std::move(other));
		return *this;
	}

	VkResult VulkanBuffer::MapMemory(void** data) const
	{
		return m_context->GetVulkanMemoryAllocator()->MapMemory(m_allocation, data);
	}

	void VulkanBuffer::UnmapMemory() const
	{
		return m_context->GetVulkanMemoryAllocator()->UnmapMemory(m_allocation);
	}

	uint32 VulkanBuffer::Size() const
	{
		return m_size;
	}

	VkBuffer VulkanBuffer::Handle() const
	{
		return m_buffer;
	}

	void VulkanBuffer::_move(VulkanBuffer&& other) noexcept
	{
		m_context = other.m_context;
		m_size = other.m_size;
		m_buffer = other.m_buffer;
		m_allocation = other.m_allocation;
		m_allocationInfo = other.m_allocationInfo;

		other.m_context = nullptr;
		other.m_size = 0;
		other.m_buffer = VK_NULL_HANDLE;
		other.m_allocation = VK_NULL_HANDLE;
		other.m_allocationInfo = {};
	}

	VkBufferUsageFlags VulkanBuffer::_getBufferUsageByBufferType(const VulkanBufferType type)
	{
		switch (type)
		{
			case VulkanBufferType::STAGING:
				return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			case VulkanBufferType::VERTEX:
				return VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			case VulkanBufferType::INDEX:
				return VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			default:
				throw std::runtime_error("Invalid Vulkan buffer type");
		}
	}

	VkMemoryPropertyFlags VulkanBuffer::_getMemoryPropertiesByBufferType(const VulkanBufferType type)
	{
		switch (type)
		{
			case VulkanBufferType::STAGING:
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case VulkanBufferType::VERTEX:
			case VulkanBufferType::INDEX:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			default:
				throw std::runtime_error("Invalid Vulkan buffer type");
		}
	}
}