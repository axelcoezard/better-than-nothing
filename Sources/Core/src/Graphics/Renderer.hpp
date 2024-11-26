#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanSwapChain;
	class VulkanRenderPass;
	class VulkanCommandPool;
	class VulkanCommandBuffer;
	class VulkanPipeline;
	class VulkanSemaphore;
	class VulkanFence;

	struct Vertex;

	class Renderer
	{
	private:
		ApplicationContext* m_context;

		std::unique_ptr<VulkanSwapChain> m_pVulkanSwapChain = nullptr;
		std::unique_ptr<VulkanRenderPass> m_pVulkanRenderPass = nullptr;
		std::unique_ptr<VulkanCommandPool> m_pVulkanCommandPool = nullptr;

		std::vector<std::unique_ptr<VulkanCommandBuffer>> m_commandBuffers;

		std::unique_ptr<VulkanPipeline> m_pPipeline = nullptr;

		std::unique_ptr<VulkanBufferFactory> m_pBufferFactory = nullptr;

		std::vector<std::unique_ptr<VulkanSemaphore>> m_imageAvailableSemaphores;
		std::vector<std::unique_ptr<VulkanSemaphore>> m_renderFinishedSemaphores;
		std::vector<std::unique_ptr<VulkanFence>> m_frameInFlightFences;

		uint32_t m_currentFrame = 0;

		const std::vector<Vertex> m_vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> m_indices = {
			0, 1, 2, 2, 3, 0
		};

		VulkanBuffer m_vertexBuffer;
		VulkanBuffer m_indexBuffer;

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
		std::unique_ptr<VulkanCommandPool>& GetVulkanCommandPool();
	};
}
