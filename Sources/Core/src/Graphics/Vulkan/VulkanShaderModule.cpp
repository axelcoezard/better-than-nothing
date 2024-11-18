#include "ApplicationContext.hpp"
#include "VulkanShaderModule.hpp"

namespace BetterThanNothing
{
	VulkanShaderModule::VulkanShaderModule(const std::string& spirvFilename, ApplicationContext* context): m_context(context)
	{
		std::vector<char> source = _readFile(spirvFilename);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = source.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

		if (vkCreateShaderModule(m_context->GetVulkanDevice()->Handle(), &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module");

		LOG_SUCCESS("Vulkan shader module '" << spirvFilename << "': ok");
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		vkDestroyShaderModule(m_context->GetVulkanDevice()->Handle(), m_shaderModule, nullptr);
	}

	std::vector<char> VulkanShaderModule::_readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("failed to open file!");

		const std::streamsize fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
};