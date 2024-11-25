//
// Created by acoezard on 11/25/24.
//

#pragma once
#include "vk_mem_alloc.h"

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanBuffer
	{
	private:
		ApplicationContext* m_context = nullptr;

		uint32_t m_size = 0;

		VkBuffer m_buffer = VK_NULL_HANDLE;

		VmaAllocation m_allocation = VK_NULL_HANDLE;
		VmaAllocationInfo m_allocationInfo = {};

	public:
		explicit VulkanBuffer(uint32_t size, VkBufferUsageFlagBits usage, ApplicationContext* context);
		~VulkanBuffer();

		VkResult MapMemory(void** data) const;
		void UnmapMemory() const;

		[[nodiscard]]
		uint32_t Size() const;

		[[nodiscard]]
		VkBuffer Handle() const;
	};
}
