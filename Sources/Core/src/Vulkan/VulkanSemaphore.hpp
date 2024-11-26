#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanSemaphore
	{
	private:
		ApplicationContext* m_context = nullptr;
		VkSemaphore m_semaphore = VK_NULL_HANDLE;

	public:
		explicit VulkanSemaphore(ApplicationContext* context);
		~VulkanSemaphore();

		[[nodiscard]]
		VkSemaphore Handle() const;
	};
}
