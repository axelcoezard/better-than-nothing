#pragma once

#include <map>
namespace BetterThanNothing
{
	class Window;
	class Device;
	class SwapChain;
	class DescriptorPool;
	class ResourceManager;
	struct Texture;
	class Pipeline;
	class Scene;
	class DrawStreamBuilder;
	struct Buffer;
	class UniformsPool;
	struct RendererDebugInfo;

	/**
	 * @brief The Renderer class that is used to render a scene
	 */
	class Renderer
	{
	private:
		/**
		 * @brief The pointer to the window
		 */
		std::shared_ptr<Window> m_Window;

		/**
		 * @brief The pointer to the device
		 */
		std::shared_ptr<Device> m_Device;

		/**
		 * @brief The pointer to the resource manager
		 */
		std::shared_ptr<ResourceManager> m_ResourceManager;

		/**
		 * @brief The pointer to the swap chain
		 */
		std::shared_ptr<SwapChain> m_SwapChain;

		/**
		 * @brief The pointer to the descriptor pool
		 */
		std::shared_ptr<DescriptorPool> m_DescriptorPool;

		/**
		 * @brief The pointer to the uniform pool
		 */
		std::shared_ptr<UniformsPool> m_UniformsPool;

		/**
		 * @brief All the pipelines
		 */
		std::map<std::string, std::shared_ptr<Pipeline>> m_PipeLines;

	public:
		/**
		 * @brief Construct a new Renderer object
		 *
		 * @param window The pointer to the window
		 * @param device The pointer to the device
		 * @param shaderPool The pointer to the shader pool
		 */
		Renderer(std::shared_ptr<Window>& window, std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager);

		/**
		 * @brief Destroy the Renderer object
		 */
		~Renderer();

	public:
		/**
		 * @brief Load a shader pipeline using the vertex and fragment shader files
		 *
		 * @param pipelineID The ID of the pipeline
		 * @param vertexShaderFilePath The path to the vertex shader file
		 * @param fragmentShaderFilePath The path to the fragment shader file
		 * @note The pipelineID must be unique.
		 * @note The vertexShaderFilePath and fragmentShaderFilePath must be valid.
		 * @note All the shaders must be compiled with the same version.
		 * @note The shader files path must be relative to the Shaders folder.
		 */
		void LoadPipeline(const std::string& pipelineID, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/**
		 * @brief Render the scene
		 * @param scene The scene to render
		 * @param debugInfo The debug info to display
		 */
		void Render(std::shared_ptr<Scene>&, RendererDebugInfo* debugInfo);

	private:
		/**
		 * @brief Render the debug info using ImGui
		 * @param debugInfo The debug info to display
		 */
		void RenderDebugInfo(RendererDebugInfo* debugInfo);

	public:
		/**
		 * @brief Get the pointer to the window
		 * @return Window* The pointer to the window
		 */
		Window* GetWindow() { return m_Window; }

		/**
		 * @brief Get the pointer to the device
		 * @return Device* The pointer to the device
		 */
		Device* GetDevice() { return m_Device; }

		/**
		 * @brief Get the pointer to the swap chain
		 * @return SwapChain* The pointer to the swap chain
		 */
		SwapChain* GetSwapChain() { return m_SwapChain; }

		/**
		 * @brief Get the pointer to the descriptor pool
		 * @return DescriptorPool* The pointer to the descriptor pool
		 */
		DescriptorPool* GetDescriptorPool() { return m_DescriptorPool; }

		/**
		 * @brief Get all the pipelines
		 * @return std::map<std::string, Pipeline*> All the pipelines
		 */
		std::map<std::string, Pipeline*> GetPipeLines() { return m_PipeLines; }
	};
};
