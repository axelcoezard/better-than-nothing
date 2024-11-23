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

		std::unique_ptr<VulkanPipeline> m_Pipeline = nullptr;

		uint32_t m_ImageIndex = 0;
	public:
		explicit Renderer(ApplicationContext* context);
		~Renderer();

		void Initialize();
		void Render();
		void AddPipeline(const std::function<void(VulkanPipelineBuilder&)>& callback);

	private:
		void _createCommandBuffers();

	public:
		std::unique_ptr<VulkanSwapChain>& GetVulkanSwapChain();
		std::unique_ptr<VulkanRenderPass>& GetVulkanRenderPass();
	};
}
