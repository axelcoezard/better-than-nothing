#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanFence
	{
	private:
		ApplicationContext* m_context = nullptr;
		VkFence m_fence = VK_NULL_HANDLE;

	public:
		explicit VulkanFence(ApplicationContext* context);
		~VulkanFence();

		void Wait();
		void Reset();

		[[nodiscard]]
		VkFence Handle() const;
	};
}
