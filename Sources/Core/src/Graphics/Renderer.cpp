#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	Renderer::Renderer(ApplicationContext* context): m_Context(context)
	{

	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Initialize()
	{
		m_pVulkanSwapChain = std::make_unique<VulkanSwapChain>(m_Context);
		m_pVulkanSwapChain->CreateImages();
		m_pVulkanSwapChain->CreateImageViews();

		m_pVulkanRenderPass = std::make_unique<VulkanRenderPass>(m_Context);

		m_pVulkanSwapChain->CreateFramebuffers();

		m_pVulkanCommandPool = std::make_unique<VulkanCommandPool>(m_Context);

		_createCommandBuffers();

		LOG_SUCCESS("Renderer: ok");
	}

	void Renderer::_createCommandBuffers()
	{
		auto device = m_Context->GetVulkanDevice()->LogicalHandle();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_pVulkanCommandPool->Handle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		m_commandBuffer = std::make_unique<VulkanCommandBuffer>(commandBuffer);
	}

	std::unique_ptr<VulkanSwapChain>& Renderer::GetVulkanSwapChain()
	{
		if (!m_pVulkanSwapChain)
			throw ApplicationContextError("Vulkan swap chain is not set");
		return m_pVulkanSwapChain;
	}

	std::unique_ptr<VulkanRenderPass>& Renderer::GetVulkanRenderPass()
	{
		if (!m_pVulkanRenderPass)
			throw ApplicationContextError("Vulkan render pass is not set");
		return m_pVulkanRenderPass;
	}
}