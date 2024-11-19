#pragma once

#include "vulkan/vulkan.h"

namespace BetterThanNothing
{
	enum class VulkanShaderType;
}

namespace BetterThanNothing
{
	class ApplicationContext;

	class VulkanShaderModule
	{
	private:
		ApplicationContext* m_context = nullptr;

		std::string m_filename;

		VulkanShaderType m_shaderType;

		glslang_stage_t m_shaderStage;

		VkShaderModule m_shaderModule = VK_NULL_HANDLE;

	public:
		explicit VulkanShaderModule(const std::string& filename, VulkanShaderType shaderType, ApplicationContext* context);
		~VulkanShaderModule();

		VulkanShaderModule(const VulkanShaderModule&) = delete;
		VulkanShaderModule& operator=(const VulkanShaderModule&) = delete;
		VulkanShaderModule(VulkanShaderModule&& other) = delete;
		VulkanShaderModule& operator=(VulkanShaderModule&& other) = delete;

		VkShaderModule Handle() const;

	private:
		static std::vector<char> _readFile(const std::string& filename);
		static glslang_stage_t _getShaderStage(VulkanShaderType type);
		static glslang_program_t* _getShaderProgram(const std::vector<char>& source, glslang_stage_t stage);
		VkShaderModule _getShaderModule(glslang_program_t* program) const;
	};
};
