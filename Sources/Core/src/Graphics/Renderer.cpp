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

	void Renderer::Render()
	{
		m_commandBuffer->BeginRecording();

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_pVulkanRenderPass->Handle();
		renderPassInfo.framebuffer = m_pVulkanSwapChain->GetFramebuffer(m_ImageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_pVulkanSwapChain->GetExtent();

		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		m_commandBuffer->CmdBeginRenderPass(&renderPassInfo);
		m_commandBuffer->CmdBindPipeline(m_Pipeline->Handle());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_pVulkanSwapChain->GetExtent().width);
		viewport.height = static_cast<float>(m_pVulkanSwapChain->GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		m_commandBuffer->CmdSetViewport(&viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = m_pVulkanSwapChain->GetExtent();
		m_commandBuffer->CmdSetScissor(&scissor);

		m_commandBuffer->CmdDraw(3, 1, 0, 0);

		m_commandBuffer->CmdEndRenderPass();
		m_commandBuffer->EndRecording();
	}

	void Renderer::AddPipeline(const std::function<void(VulkanPipelineBuilder&)>& callback)
	{
		VulkanPipelineBuilder builder;
		callback(builder);

		m_Pipeline = std::make_unique<VulkanPipeline>(builder.GetBuildParams(), m_Context);
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