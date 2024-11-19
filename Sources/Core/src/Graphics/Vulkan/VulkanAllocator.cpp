//
// Created by acoezard on 11/19/24.
//

#include "ApplicationContext.hpp"
#include "VulkanAllocator.hpp"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

namespace BetterThanNothing
{
	VulkanAllocator::VulkanAllocator(ApplicationContext* pContext)
	{
		(void) pContext;
		// VmaVulkanFunctions vulkanFunctions = {};
		// vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		// vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
		//
		// VmaAllocatorCreateInfo allocatorCreateInfo = {};
		// allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		// allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2; // TODO: Put that in the context
		// allocatorCreateInfo.physicalDevice = pContext->GetVulkanDevice()->PhysicalHandle();
		// allocatorCreateInfo.device = pContext->GetVulkanDevice()->LogicalHandle();
		// allocatorCreateInfo.instance = pContext->GetVulkanInstance()->Handle();
		// allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
		//
		// vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);
	}

	VulkanAllocator::~VulkanAllocator()
	{
		// vmaDestroyAllocator(m_allocator);
	}

	VmaAllocator VulkanAllocator::Handle() const
	{
		return m_allocator;
	}
}