#pragma once

namespace BetterThanNothing
{
	class Device;
	class SwapChain;
	class DescriptorPool;
	struct Shader;

	/**
	 * @brief A pipeline uses shaders and a render pass to describe the graphics pipeline
	 */
	class Pipeline
	{
	private:
		/**
		 * @brief The id of the pipeline
		 */
		std::string			m_Id;

		/**
		 * @brief A pointer to the device
		 */
		std::shared_ptr<Device> m_Device;

		/**
		 * @brief A pointer to the swap chain
		 */
		std::shared_ptr<SwapChain> m_SwapChain;

		/**
		 * @brief A pointer to the descriptor pool
		 */
		std::shared_ptr<DescriptorPool> m_DescriptorPool;

		/**
		 * @brief The Pipeline Layout
		 */
		VkPipelineLayout	m_PipelineLayout;

		/**
		 * @brief The graphics pipeline
		 */
		VkPipeline			m_GraphicsPipeline;

	public:
		/**
		 * @brief Construct a new Pipeline object
		 *
		 * @param id The id of the pipeline
		 * @param device A pointer to the device
		 * @param swapChain A pointer to the swap chain
		 * @param descriptorPool A pointer to the descriptor pool
		 */
		Pipeline(const std::string& id, std::shared_ptr<Device>& device, std::shared_ptr<SwapChain>& swapChain, std::shared_ptr<DescriptorPool>& descriptorPool);

		/**
		 * @brief Destroy the Pipeline object
		 */
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;
		Pipeline(Pipeline&&) = delete;
		Pipeline& operator=(Pipeline&&) = delete;

	private:
		/**
		 * @brief Create the render pass
		 */
		void CreateRenderPass();

		/**
		 * @brief Create the descriptor set layout
		 */
		void CreateDescriptorSetLayout();

	public:
		/**
		 * @brief Create the graphics pipeline
		 */
		void CreateGraphicsPipeline(Shader* vertexShader, Shader* fragmentShader);

		/**
		 * @brief Get the id of the pipeline
		 *
		 * @return std::string The id of the pipeline
		 */
		std::string& GetId() { return m_Id; }

		/**
		 * @brief Get the pipeline layout
		 */
		VkPipelineLayout& GetVkPipelineLayout() { return m_PipelineLayout; }

		/**
		 * @brief Get the graphics pipeline
		 */
		VkPipeline& GetVkGraphicsPipeline() { return m_GraphicsPipeline; }
	};
}
