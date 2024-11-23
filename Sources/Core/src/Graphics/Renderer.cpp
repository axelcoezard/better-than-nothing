#include "ApplicationContext.hpp"
#include "Renderer.hpp"

namespace BetterThanNothing
{
	Renderer::Renderer(ApplicationContext* context): m_Context(context)
	{
		(void) m_Context;
	}

	Renderer::~Renderer()
	{
	}
}