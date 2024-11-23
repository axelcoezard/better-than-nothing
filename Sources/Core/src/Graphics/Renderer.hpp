#pragma once

#include "Graphics/Vulkan/VulkanSwapChain.hpp"
#include "Graphics/Vulkan/VulkanRenderPass.hpp"
#include "Graphics/Vulkan/VulkanCommandPool.hpp"
#include "Graphics/Vulkan/VulkanCommandBuffer.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

	class Renderer
	{
	private:
		ApplicationContext* m_Context;

		std::unique_ptr<VulkanSwapChain> m_pVulkanSwapChain = nullptr;
		std::unique_ptr<VulkanRenderPass> m_pVulkanRenderPass = nullptr;
		std::unique_ptr<VulkanCommandPool> m_pVulkanCommandPool = nullptr;
		std::unique_ptr<VulkanCommandBuffer> m_commandBuffer = nullptr;

		std::unique_ptr<VulkanPipeline> m_pPipeline = nullptr;

		std::unique_ptr<VulkanSemaphore> m_pImageAvailableSemaphore = nullptr;
		std::unique_ptr<VulkanSemaphore> m_pRenderFinishedSemaphore = nullptr;
		std::unique_ptr<VulkanFence> m_pFrameInFlightFence = nullptr;

	public:
		explicit Renderer(ApplicationContext* context);
		~Renderer();

		void Initialize();
		void Render();
		void AddPipeline(const std::function<void(VulkanPipelineBuilder&)>& callback);

	private:
		void _createCommandBuffers();
		void _createSyncObjects();
		void _recordCommandBuffer(uint32_t imageIndex);

	public:
		std::unique_ptr<VulkanSwapChain>& GetVulkanSwapChain();
		std::unique_ptr<VulkanRenderPass>& GetVulkanRenderPass();
	};
}
