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

		[[nodiscard]]
		VmaAllocator Handle() const;
	};
};
