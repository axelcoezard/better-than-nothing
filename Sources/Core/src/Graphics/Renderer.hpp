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
	public:
		explicit Renderer(ApplicationContext* context);
		~Renderer();

		void Initialize();

	private:
		void _createCommandBuffers();

	public:
		std::unique_ptr<VulkanSwapChain>& GetVulkanSwapChain();
		std::unique_ptr<VulkanRenderPass>& GetVulkanRenderPass();
	};
}
