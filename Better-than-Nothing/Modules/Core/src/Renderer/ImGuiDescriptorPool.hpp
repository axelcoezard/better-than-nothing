#pragma once

namespace BetterThanNothing
{
	class Device;

	class ImGuiDescriptorPool
	{
	private:
		std::shared_ptr<Device> m_Device;

		VkDescriptorPool m_DescriptorPool;

	public:
		ImGuiDescriptorPool(std::shared_ptr<Device>& device);
		~ImGuiDescriptorPool();

		VkDescriptorPool& GetVkDescriptorPool();
	};
};
