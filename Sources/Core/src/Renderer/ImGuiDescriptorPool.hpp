#pragma once

namespace BetterThanNothing
{
	class Device;

	class ImGuiDescriptorPool
	{
	private:
		std::unique_ptr<Device>& m_Device;

		VkDescriptorPool m_DescriptorPool;

	public:
		ImGuiDescriptorPool(std::unique_ptr<Device>& device);
		~ImGuiDescriptorPool();

		VkDescriptorPool& GetVkDescriptorPool();
	};
};
