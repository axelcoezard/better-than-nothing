//
// Created by acoezard on 11/19/24.
//

#include "ApplicationContext.hpp"
#include "VulkanAllocator.hpp"


namespace BetterThanNothing
{
	VulkanAllocator::VulkanAllocator(ApplicationContext* pContext)
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

	VulkanAllocator::~VulkanAllocator()
	{
		 vmaDestroyAllocator(m_allocator);
	}

	VmaAllocator VulkanAllocator::Handle() const
	{
		return m_allocator;
	}
}