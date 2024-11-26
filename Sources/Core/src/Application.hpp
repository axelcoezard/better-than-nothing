#pragma once

#include "Window.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

	class Application
	{
	protected:
		Window m_window;

	public:

		Application() = default;
		virtual ~Application() = default;

		void Run();

		virtual void OnEnable(ApplicationContext* context) = 0;
		virtual void OnUpdate(ApplicationContext* context) = 0;
		virtual void OnRender(ApplicationContext* context) = 0;
		virtual void OnDisable(ApplicationContext* context) = 0;
	};
};
