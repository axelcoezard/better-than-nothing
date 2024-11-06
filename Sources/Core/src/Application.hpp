#pragma once

#include "RAII/Window.hpp"

namespace BetterThanNothing
{
	class Event;

	/**
	 * @brief The Application class is the main class of the engine's applications.
	 */
	class Application
	{
	protected:
		/**
		 * @brief A pointer to the Window
		*/
		Window m_window;

		std::atomic<bool> m_running = true;
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
	};
};
