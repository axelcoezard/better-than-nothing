#include "ApplicationContext.hpp"
#include "VulkanCommandBuffer.hpp"

namespace BetterThanNothing
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool commandPool, ApplicationContext* context)
		: m_context(context), m_commandPool(commandPool)
	{
		auto device = m_context->GetVulkanDevice()->LogicalHandle();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device, &allocInfo, &m_CommandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(m_context->GetVulkanDevice()->LogicalHandle(), m_commandPool, 1, &m_CommandBuffer);
	}

	void VulkanCommandBuffer::BeginRecording()
	{
		if (m_bRecording)
			throw std::runtime_error("Command buffer is already recording");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		m_bRecording = true;
	}

	void VulkanCommandBuffer::EndRecording()
	{
		if (!m_bRecording)
			throw std::runtime_error("Command buffer is not recording");

		if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");

		m_bRecording = false;
	}

	VkCommandBuffer VulkanCommandBuffer::Handle() const
	{
		return m_CommandBuffer;
	}
}