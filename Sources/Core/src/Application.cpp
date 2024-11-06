#include "BetterThanNothing.hpp"
#include "RAII/Vulkan/VulkanInstance.hpp"

namespace BetterThanNothing
{
	Application::Application()
	{
		ConfigManager configManager("../../Config/");

		ConfigReader config = configManager.Read("Config.ini");
		const std::string windowTitle = config->Get("window", "title", "Application");
		const long windowWidth = config->GetInteger("window", "width", 800);
		const long windowHeight = config->GetInteger("window", "height", 600);

		m_window = Window(windowTitle, windowWidth, windowHeight);
		m_window.SetEventCallback(BIND_EVENT_LISTENER(OnEvent));
	}

	Application::~Application() = default;

	void Application::Run()
	{
		OnEnable();

		JobGraph jobGraph("Main");

		const auto gameplayJob = jobGraph.AddNode("Gameplay", []() {
			//std::cout << std::this_thread::get_id() << " - Gameplay" << std::endl;
		});

		const auto renderJob = jobGraph.AddNode("Render", []() {
			//std::cout << std::this_thread::get_id() << " - Render" << std::endl;
		});

		renderJob->AddDependency(gameplayJob);

		ApplicationContext context = {
			.window = m_window,
			.vulkanInstance = VulkanInstance(false)
		};

		VulkanSurface vulkanSurface(&context);

		JobManager jobManager;

		while (m_running)
		{
			m_window.Poll();

			if (m_window.ShouldClose())
			{
				m_window.Close();
				m_running.store(false);
				continue;
			}

			jobManager.ExecuteGraph(jobGraph);

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		jobManager.Stop();

		OnDisable();
	}

	void Application::OnEvent(Event* event)
	{
		(void) event;
	}
};
