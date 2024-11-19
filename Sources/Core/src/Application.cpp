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
			.SetShadersFolderPath("../../Assets/Shaders/")
			.Build();

		context.Initialize();

		auto vert = context.LoadShader("main/main.vert", VulkanShaderType::Vertex);
		auto frag = context.LoadShader("main/main.frag", VulkanShaderType::Fragment);

		VulkanPipeline pipeline = VulkanPipelineBuilder()
			.AddShader(vert)
			.AddShader(frag)
			.Build(&context);

		this->OnEnable();

		while (!context.GetWindow()->ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		this->OnDisable();
	}
};
