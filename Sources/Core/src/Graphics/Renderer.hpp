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
		ApplicationContext* m_context;

		std::unique_ptr<VulkanSwapChain> m_pVulkanSwapChain = nullptr;
		std::unique_ptr<VulkanRenderPass> m_pVulkanRenderPass = nullptr;
		std::unique_ptr<VulkanCommandPool> m_pVulkanCommandPool = nullptr;

		std::vector<std::unique_ptr<VulkanCommandBuffer>> m_commandBuffers;

		std::unique_ptr<VulkanPipeline> m_pPipeline = nullptr;

		std::vector<std::unique_ptr<VulkanSemaphore>> m_imageAvailableSemaphores;
		std::vector<std::unique_ptr<VulkanSemaphore>> m_renderFinishedSemaphores;
		std::vector<std::unique_ptr<VulkanFence>> m_frameInFlightFences;

		uint32_t m_currentFrame = 0;

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
