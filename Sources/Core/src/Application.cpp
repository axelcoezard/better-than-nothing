#include "BetterThanNothing.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"

namespace BetterThanNothing
{
	void Application::Run()
	{
		ApplicationContext context;

		context.InitWindow("Better Than Nothing", 800, 600);
		context.InitVulkan(true);

		this->OnEnable();

		while (!context.GetWindow()->ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		context.SetRunning(false);

		this->OnDisable();
	}
};
