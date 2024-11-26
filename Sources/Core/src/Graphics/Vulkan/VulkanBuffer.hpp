//
// Created by acoezard on 11/25/24.
//

#pragma once

#include "Graphics/Vulkan/VulkanBufferType.hpp"

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
		VulkanBuffer() = default;
		VulkanBuffer(uint32_t size, VulkanBufferType type, ApplicationContext* context);
		~VulkanBuffer();

		VulkanBuffer(const VulkanInstance&) = delete;
		VulkanBuffer& operator=(const VulkanBuffer&) = delete;

		VulkanBuffer(VulkanBuffer&&) noexcept;
		VulkanBuffer& operator=(VulkanBuffer&&) noexcept;

		VkResult MapMemory(void** data) const;
		void UnmapMemory() const;

		[[nodiscard]]
		uint32_t Size() const;

		[[nodiscard]]
		VkBuffer Handle() const;

	private:
		void _move(VulkanBuffer&& other) noexcept;
	};
}
