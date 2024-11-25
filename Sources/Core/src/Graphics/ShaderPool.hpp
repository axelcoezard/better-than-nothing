//
// Created by acoezard on 11/19/24.
//

#pragma once

#include "Vulkan/VulkanShaderType.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;
	struct VulkanShaderModule;

	class ShaderPool
	{
	private:
		ApplicationContext* m_context;

		std::string m_basePath;
		std::unordered_map<std::string, VulkanShaderModule> m_shaders;

	public:
		explicit ShaderPool(std::string  basePath, ApplicationContext* context): m_context(context), m_basePath(std::move(basePath)) {}
		~ShaderPool();

		VulkanShaderModule LoadShader(const std::string& path, VulkanShaderType type);
	private:
		static std::vector<char> _readFile(const std::string& filename);
		static glslang_stage_t _getShaderStage(VulkanShaderType type);
		static glslang_program_t* _getShaderProgram(const std::vector<char>& source, glslang_stage_t stage);
		VkShaderModule _getShaderModule(glslang_program_t* program) const;
	};
};

