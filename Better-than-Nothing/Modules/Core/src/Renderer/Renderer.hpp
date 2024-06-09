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
		std::unique_ptr<Window>& m_Window;

		/**
		 * @brief The pointer to the device
		 */
		std::unique_ptr<Device>& m_Device;

		/**
		 * @brief The pointer to the resource manager
		 */
		std::unique_ptr<ResourceManager>& m_ResourceManager;

		/**
		 * @brief The pointer to the swap chain
		 */
		std::unique_ptr<SwapChain> m_SwapChain;

		/**
		 * @brief The pointer to the descriptor pool
		 */
		std::unique_ptr<DescriptorPool> m_DescriptorPool;

		/**
		 * @brief The pointer to the uniform pool
		 */
		std::unique_ptr<UniformsPool> m_UniformsPool;

		/**
		 * @brief All the pipelines
		 */
		std::map<std::string, std::unique_ptr<Pipeline>> m_PipeLines;

	public:
		/**
		 * @brief Construct a new Renderer object
		 *
		 * @param window The pointer to the window
		 * @param device The pointer to the device
		 * @param shaderPool The pointer to the shader pool
		 */
		Renderer(std::unique_ptr<Window>& window, std::unique_ptr<Device>& device, std::unique_ptr<ResourceManager>& resourceManager);

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
		void Render(Scene*, RendererDebugInfo* debugInfo);

	private:
		/**
		 * @brief Render the debug info using ImGui
		 * @param debugInfo The debug info to display
		 */
		void RenderDebugInfo(RendererDebugInfo* debugInfo);
	};
};
