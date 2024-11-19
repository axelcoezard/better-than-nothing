//
// Created by acoezard on 11/19/24.
//

#pragma once

namespace BetterThanNothing
{
	class ApplicationContext;
	class VulkanShaderModule;


	class ShaderPool
	{
	private:
		ApplicationContext* m_context;

		std::string m_basePath;
		std::unordered_map<std::string, std::unique_ptr<VulkanShaderModule>> m_shaders;

	public:
		explicit ShaderPool(std::string  basePath, ApplicationContext* context);
		~ShaderPool() = default;

		VkShaderModule LoadShader(const std::string& name, VulkanShaderType type);
	};
};

