#pragma once

#include "Graphics/Window.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

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
		virtual void OnEnable(ApplicationContext* context) = 0;

		/**
		 * @brief Called when the Application is disabled
		*/
		virtual void OnDisable(ApplicationContext* context) = 0;
	};
};
