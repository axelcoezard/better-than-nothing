#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	Application::Application()
	{
		m_ConfigManager = std::make_unique<ConfigManager>("../../Config/");

		ConfigReader config = m_ConfigManager->Read("Config.ini");
		std::string windowTitle = config->Get("window", "title", "Application");
		u32 windowWidth = config->GetInteger("window", "width", 800);
		u32 windowHeight = config->GetInteger("window", "height", 600);

		m_Window = std::make_unique<Window>(windowTitle, windowWidth, windowHeight);
		m_Window->SetEventCallback(BIND_EVENT_LISTENER(OnEvent));

		m_Device = std::make_unique<Device>(m_Window);

		m_ResourceManager = std::make_unique<ResourceManager>(m_Device, "../../Assets/");

		m_Renderer = std::make_unique<Renderer>(m_Window, m_Device, m_ResourceManager);
	}

	Application::~Application()
	{
		m_Scenes.clear();
	}

	void Application::Run()
	{
		OnEnable();

		m_gameplayThread.thread = std::thread(&Application::GameplayThread, this);
		m_renderThread.thread = std::thread(&Application::RenderThread, this);

		while (m_running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds (10));
		}

		m_running = false;

		m_gameplayThread.thread.join();
		m_renderThread.thread.join();

		OnDisable();
	}

	void Application::GameplayThread()
	{
		while (m_running)
		{
			m_Window->Poll();
			m_running = !m_Window->ShouldClose();

			double currentFrame = glfwGetTime();
			m_gameplayThread.deltatime = currentFrame - m_gameplayThread.lastFrame;
			m_gameplayThread.lastFrame = currentFrame;
			m_gameplayThread.frameTime = 1.0 / 240.0;
			m_gameplayThread.frameCount += 1;

			auto currentScene = m_Scenes.at(m_CurrentSceneId);
			currentScene->OnUpdate(m_gameplayThread.deltatime);

			auto frameTimeMicroseconds = static_cast<useconds_t>(m_gameplayThread.frameTime * 1000000);
			double elapsedTime = glfwGetTime() - currentFrame;
			if (elapsedTime < m_gameplayThread.frameTime) {
				std::this_thread::sleep_for(
					std::chrono::microseconds(frameTimeMicroseconds - static_cast<useconds_t>(elapsedTime * 1000000))
				);
			}
		}
	}

	void Application::RenderThread()
	{
		while (m_running)
		{
			double currentFrame = glfwGetTime();
			m_renderThread.deltatime = currentFrame - m_renderThread.lastFrame;
			m_renderThread.lastFrame = currentFrame;
			m_renderThread.frameTime = 1.0 / 240.0;
			m_renderThread.frameCount += 1;

			auto currentScene = m_Scenes.at(m_CurrentSceneId);

			RendererDebugInfo debugInfo{};
			debugInfo.vendorName = m_Device->GetVendorName();
			debugInfo.deviceName = m_Device->GetDeviceName();
			debugInfo.apiVersion = m_Device->GetApiVersion();
			debugInfo.frameCount = m_renderThread.frameCount;
			debugInfo.deltaTime = m_renderThread.deltatime;
			debugInfo.sceneName = currentScene->GetName();
			debugInfo.sceneEntitiesCount = currentScene->GetEntitiesCount();

			m_Renderer->Render(currentScene, &debugInfo);
			m_Device->WaitIdle();

			auto frameTimeMicroseconds = static_cast<useconds_t>(m_renderThread.frameTime * 1000000);
			double elapsedTime = glfwGetTime() - currentFrame;
			if (elapsedTime < m_renderThread.frameTime) {
				std::this_thread::sleep_for(
					std::chrono::microseconds(frameTimeMicroseconds - static_cast<useconds_t>(elapsedTime * 1000000))
				);
			}
		}
	}

	void Application::OnEvent(Event* event)
	{
		m_Scenes[m_CurrentSceneId]->OnEvent(event);
	}

	Scene* Application::CreateScene(const std::string& name)
	{
		auto scene = new Scene(m_Scenes.size(), name, m_Window, m_ResourceManager);
		m_Scenes.push_back(scene);
		m_CurrentSceneId = scene->GetId();
		return scene;
	}
};
