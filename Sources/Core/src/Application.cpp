#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	Application::Application()
	{
		ConfigManager configManager("../../Config/");

		ConfigReader config = configManager.Read("Config.ini");
		std::string windowTitle = config->Get("window", "title", "Application");
		u32 windowWidth = config->GetInteger("window", "width", 800);
		u32 windowHeight = config->GetInteger("window", "height", 600);

		m_window.Init(windowTitle, windowWidth, windowHeight);
		m_window.SetEventCallback(BIND_EVENT_LISTENER(OnEvent));
	}

	Application::~Application() = default;

	void Application::Run()
	{
		OnEnable();

		JobGraph jobGraph("Main");

		auto gameplayJob = jobGraph.AddNode("Gameplay", []() {
			std::cout << std::this_thread::get_id() << " - Gameplay" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds (12000));
		});

		auto renderJob = jobGraph.AddNode("Render", []() {
			std::cout << std::this_thread::get_id() << " - Render" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds (12000));
		});

		renderJob->AddDependency(gameplayJob);

		JobManager jobManager;

		jobManager.ExecuteGraph(jobGraph);

		while (m_running)
		{
			m_window.Poll();

			if (m_window.ShouldClose())
			{
				m_running.store(false);

				std::cout << "Not running anymore" << std::endl;
				continue;
			}

			std::cout << std::this_thread::get_id() << " - Main" << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}
		std::cout << "Before window close" << std::endl;

		m_window.Close();
		std::cout << "After window close" << std::endl;

		jobManager.Stop();
		std::cout << "After job manager stop" << std::endl;

		OnDisable();
	}


	void Application::OnEvent(Event* event)
	{
		(void) event;
	}
};
