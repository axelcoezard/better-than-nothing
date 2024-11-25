//
// Created by acoezard on 11/19/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanMemoryAllocator::VulkanMemoryAllocator(ApplicationContext* pContext)
	{
		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		allocatorCreateInfo.vulkanApiVersion = pContext->GetVulkanApiVersion();
		allocatorCreateInfo.physicalDevice = pContext->GetVulkanDevice()->PhysicalHandle();
		allocatorCreateInfo.device = pContext->GetVulkanDevice()->LogicalHandle();
		allocatorCreateInfo.instance = pContext->GetVulkanInstance()->Handle();
		allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

		if (vmaCreateAllocator(&allocatorCreateInfo, &m_allocator) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan allocator");

		LOG_SUCCESS("Vulkan memory allocator (Vma): ok");
	}

	VkResult VulkanMemoryAllocator::CreateBuffer(const VkBufferCreateInfo* bufferCreateInfo,
		VkBuffer* buffer, VmaAllocation* allocation, VmaAllocationInfo* allocationInfo) const
	{
		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		// Use: allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT; to map the memory automatically

		return vmaCreateBuffer(m_allocator, bufferCreateInfo, &allocInfo, buffer, allocation, allocationInfo);
	}

	void VulkanMemoryAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation) const
	{
		vmaDestroyBuffer(m_allocator, buffer, allocation);
	}

	VkResult VulkanMemoryAllocator::MapMemory(VmaAllocation allocation, void** data) const
	{
		return vmaMapMemory(m_allocator, allocation, data);
	}

	void VulkanMemoryAllocator::UnmapMemory(VmaAllocation allocation) const
	{
		return vmaUnmapMemory(m_allocator, allocation);
	}

	VulkanMemoryAllocator::~VulkanMemoryAllocator()
	{
		 vmaDestroyAllocator(m_allocator);
	}

	VmaAllocator VulkanMemoryAllocator::Handle() const
	{
		return m_allocator;
	}
}