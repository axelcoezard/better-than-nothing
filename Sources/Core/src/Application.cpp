#include "BetterThanNothing.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"

namespace BetterThanNothing
{
	void Application::Run()
	{
		ApplicationContext context = {
			.running = true,
			.window = Window("Better Than Nothing", 800, 600),
			.vulkanInstance = VulkanInstance(false)
		};

		OnEnable();

		while (!context.window.ShouldClose())
		{
			Window::PollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		context.running = false;

		OnDisable();
	}
};
