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
	public:

		Application() = default;
		virtual ~Application() = default;

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
	};
};
