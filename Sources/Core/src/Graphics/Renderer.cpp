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
		_createSyncObjects();

		LOG_SUCCESS("Renderer: ok");
	}

	void Renderer::Render()
	{
		m_pFrameInFlightFence->Wait();
		m_pFrameInFlightFence->Reset();

		auto device = m_Context->GetVulkanDevice()->LogicalHandle();
		auto swapchain = m_pVulkanSwapChain->Handle();

		uint32_t imageIndex;
		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, m_pImageAvailableSemaphore->Handle(), VK_NULL_HANDLE, &imageIndex);

		_recordCommandBuffer(imageIndex);

		VkSemaphore waitSemaphores[] = { m_pImageAvailableSemaphore->Handle() };

		// We wait for the fragment shader to output the color before we can start writing to the image
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		auto commandBuffer = m_commandBuffer->Handle();
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { m_pRenderFinishedSemaphore->Handle() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		auto graphicsQueue = m_Context->GetGraphicsQueue()->Handle();
		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, m_pFrameInFlightFence->Handle()) != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_Context->GetPresentQueue()->Handle(), &presentInfo);
	}

	void Renderer::_recordCommandBuffer(uint32_t imageIndex)
	{
		m_commandBuffer->Reset();
		m_commandBuffer->BeginRecording();

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_pVulkanRenderPass->Handle();
		renderPassInfo.framebuffer = m_pVulkanSwapChain->GetFramebuffer(imageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_pVulkanSwapChain->GetExtent();

		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		m_commandBuffer->CmdBeginRenderPass(&renderPassInfo);
		m_commandBuffer->CmdBindPipeline(m_pPipeline->Handle());

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

		m_pPipeline = std::make_unique<VulkanPipeline>(builder.GetBuildParams(), m_Context);
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

	void Renderer::_createSyncObjects()
	{
		m_pImageAvailableSemaphore = std::make_unique<VulkanSemaphore>(m_Context);
		m_pRenderFinishedSemaphore = std::make_unique<VulkanSemaphore>(m_Context);
		m_pFrameInFlightFence = std::make_unique<VulkanFence>(m_Context);
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