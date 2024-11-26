//
// Created by acoezard on 11/19/24.
//

#pragma once

#include <vk_mem_alloc.h>

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanMemoryAllocator
	{
	private:
		VmaAllocator m_allocator = VK_NULL_HANDLE;

	public:
		explicit VulkanMemoryAllocator(ApplicationContext* pContext);
		~VulkanMemoryAllocator();

		VkResult CreateBuffer(const VkBufferCreateInfo* bufferCreateInfo, VkMemoryPropertyFlags properties, VkBuffer* buffer, VmaAllocation* allocation, VmaAllocationInfo* allocationInfo) const;
		void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation) const;

		VkResult CreateImage(const VkImageCreateInfo* imageCreateInfo, VkImage* image, VmaAllocation* allocation, VmaAllocationInfo* allocationInfo) const;
		VkResult AllocateMemory(const VkMemoryRequirements* memoryRequirements, VmaAllocation* allocation, VmaAllocationInfo* allocationInfo) const;
		VkResult MapMemory(VmaAllocation allocation, void** data) const;
		void UnmapMemory(VmaAllocation allocation) const;

		[[nodiscard]]
		VmaAllocator Handle() const;
	};
};
