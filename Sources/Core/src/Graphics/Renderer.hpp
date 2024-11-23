#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class Renderer
	{
	private:
		ApplicationContext* m_Context;

		std::unique_ptr<VulkanCommandPool> m_pVulkanCommandPool = nullptr;
	public:
		explicit Renderer(ApplicationContext* context);
		~Renderer();
	};
}
