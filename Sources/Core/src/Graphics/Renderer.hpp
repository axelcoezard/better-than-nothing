#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class Renderer
	{
	private:
		ApplicationContext* m_Context;

	public:
		explicit Renderer(ApplicationContext* context);
		~Renderer();
	};
}
