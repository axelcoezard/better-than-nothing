#include "ApplicationContext.hpp"
#include "Renderer.hpp"

namespace BetterThanNothing
{
	Renderer::Renderer(ApplicationContext* context): m_Context(context)
	{
		m_pVulkanCommandPool = std::make_unique<VulkanCommandPool>(m_Context);
	}

	Renderer::~Renderer()
	{
	}
}