#pragma once

#include "vulkan/vulkan.h"

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanShaderModule
	{
	private:
		ApplicationContext* m_context = nullptr;
		VkShaderModule m_shaderModule = VK_NULL_HANDLE;

	public:
		explicit VulkanShaderModule(const std::string& filename, ApplicationContext* context);
		~VulkanShaderModule();

		VulkanShaderModule(const VulkanShaderModule&) = delete;
		VulkanShaderModule& operator=(const VulkanShaderModule&) = delete;
		VulkanShaderModule(VulkanShaderModule&& other) = delete;
		VulkanShaderModule& operator=(VulkanShaderModule&& other) = delete;

		[[nodiscard]]
		VkShaderModule Handle() const { return m_shaderModule; }

	private:
		static std::vector<char> _readFile(const std::string& filename);
	};
};
