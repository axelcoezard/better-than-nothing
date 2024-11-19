//
// Created by acoezard on 11/19/24.
//

#pragma once

#include <vk_mem_alloc.h>

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanAllocator
	{
	private:
		VmaAllocator m_allocator = VK_NULL_HANDLE;

	public:
		explicit VulkanAllocator(ApplicationContext* pContext);
		~VulkanAllocator();

		[[nodiscard]]
		VmaAllocator Handle() const;
	};
};
