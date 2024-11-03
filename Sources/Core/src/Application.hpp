#pragma once

namespace BetterThanNothing
{
	class ConfigManager;
	class Window;
	class Event;

	struct ThreadContext {
		std::thread thread;

		double deltatime = 0.0f;
		double lastFrame = 0.0f;
		double frameTime = 1.0f / 240.0f;
		uint32_t frameCount = 0;
	};

	/**
	 * @brief The Application class is the main class of the engine's applications.
	 */
	class Application
	{
	protected:
		/**
		 * @brief A pointer to the ConfigManager
		*/
		std::unique_ptr<ConfigManager> m_ConfigManager;

		/**
		 * @brief A pointer to the Window
		*/
		std::unique_ptr<Window> m_Window;

		/**
		 * @brief The current Scene of the Application
		*/
		u32 m_CurrentSceneId;

		std::atomic<bool> m_running = true;

		ThreadContext m_gameplayThread;
		ThreadContext m_renderThread;
	public:
		/**
		 * @brief Construct a new Application object
		 * @param title The title of the window
		 * @param width The width of the window
		 * @param height The height of the window
		*/
		Application();

		/**
		 * @brief Destroy the Application object
		*/
		virtual ~Application();

		/**
		 * @brief Run the Application
		*/
		void Run();

		/**
		 * @brief Called when the Application is enabled
		*/
		virtual void OnEnable() = 0;

		/**
		 * @brief Called when the Application is disabled
		*/
		virtual void OnDisable() = 0;

		/**
		 * @brief Called when an Event is received
		 * @param event A pointer to the Event object received
		*/
		void OnEvent(Event* event);

	private:
		void GameplayThread();
		void RenderThread();
	};
};
