#include "BetterThanNothing.hpp"
#include "Graphics/ShaderPool.hpp"

namespace BetterThanNothing
{
	void Application::Run()
	{
		ApplicationContext context = ApplicationContextBuilder()
			.SetWindowTitle("Better Than Nothing")
			.SetWindowSize(800, 600)
			.EnableValidationLayers(true)
			.Build();

		context.Initialize();

		ShaderPool shaderPool("../../Assets/Shaders/", &context);
		shaderPool.LoadShader("main/main.vert", VulkanShaderType::Vertex);

		// VulkanPipeline pipeline = VulkanPipelineBuilder(&context)
		// 	.AddShader("shaders/vert.spv", VulkanShaderType::Vertex)
		// 	.AddShader("shaders/frag.spv", VulkanShaderType::Fragment)
		// 	.Build();

		// (void) pipeline;

		this->OnEnable();

		while (!context.GetWindow()->ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		this->OnDisable();
	}
};
