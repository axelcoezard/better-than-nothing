#include "BetterThanNothing.hpp"

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

		this->OnEnable();

		while (!context.GetWindow()->ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		this->OnDisable();
	}
};
