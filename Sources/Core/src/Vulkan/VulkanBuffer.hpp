//
// Created by acoezard on 11/25/24.
//

#pragma once

#include "Vulkan/VulkanBufferType.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanBuffer
	{
	private:
		ApplicationContext* m_context = nullptr;

		uint32 m_size = 0;

		VkBuffer m_buffer = VK_NULL_HANDLE;

		VmaAllocation m_allocation = VK_NULL_HANDLE;
		VmaAllocationInfo m_allocationInfo = {};

	public:
		VulkanBuffer() = default;
		VulkanBuffer(uint32 size, VulkanBufferType type, ApplicationContext* context);
		~VulkanBuffer();

		VulkanBuffer(const VulkanInstance&) = delete;
		VulkanBuffer& operator=(const VulkanBuffer&) = delete;

		VulkanBuffer(VulkanBuffer&&) noexcept;
		VulkanBuffer& operator=(VulkanBuffer&&) noexcept;

		VkResult MapMemory(void** data) const;
		void UnmapMemory() const;

		[[nodiscard]]
		uint32 Size() const;

		[[nodiscard]]
		VkBuffer Handle() const;

	private:
		void _move(VulkanBuffer&& other) noexcept;

		static VkBufferUsageFlags _getBufferUsageByBufferType(VulkanBufferType type);
		static VkMemoryPropertyFlags _getMemoryPropertiesByBufferType(VulkanBufferType type);
	};
}
