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
		{
			auto gameplayJob = jobGraph.AddNode("Gameplay", []() {
				LOG_INFO("Gameplay");
			});

			auto renderJob = jobGraph.AddNode("Render", []() {
				LOG_INFO("Render");
			});

			renderJob->AddDependency(gameplayJob);
		}

		JobManager jobManager;

		while (m_running)
		{
			m_window.Poll();

			if (m_window.ShouldClose())
			{
				m_running.store(false);
				continue;
			}

			jobManager.ExecuteGraph(jobGraph);

			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		jobManager.Stop();

		m_running = false;

		OnDisable();
	}


	void Application::OnEvent(Event* event)
	{
		(void) event;
	}
};
