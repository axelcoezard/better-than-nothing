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
			.SetBufferingType(VulkanBufferingType::TripleBuffering)
			.EnableVSync(true)
			.Build();

		context.Initialize();

		this->OnEnable(&context);

		context.GetWindow()->Show();

		while (!context.GetWindow()->ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		this->OnDisable(&context);
	}
};
