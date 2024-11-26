//
// Created by acoezard on 11/25/24.
//

#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	enum class VulkanBufferType;
	class VulkanBuffer;

	class VulkanBufferFactory
	{
	private:
		ApplicationContext* m_context;

	public:
		explicit VulkanBufferFactory(ApplicationContext* context): m_context(context) {}
		~VulkanBufferFactory() = default;

		VulkanBuffer CreateVertexBuffer(const void* data, uint32_t dataSize) const;
	};
}
