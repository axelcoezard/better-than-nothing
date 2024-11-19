//
// Created by acoezard on 11/19/24.
//

#pragma once

#include "VulkanShaderType.hpp"
#include "VulkanShaderModule.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

	struct VulkanPipelineParams
	{
		std::string name;
		std::vector<VulkanShaderModule> shaders;
	};

	class VulkanPipeline
	{
	private:
		ApplicationContext* m_context;
		VulkanPipelineParams m_params;

		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

	public:
		explicit VulkanPipeline(VulkanPipelineParams  params, ApplicationContext* context);
		~VulkanPipeline();

	private:
		static VkShaderStageFlagBits _getShaderStageTypeByShaderType(VulkanShaderType type);
	};

	class VulkanPipelineBuilder
	{
	private:
		VulkanPipelineParams m_params{};

	public:
		explicit VulkanPipelineBuilder() = default;
		~VulkanPipelineBuilder() = default;

		VulkanPipelineBuilder& SetName(const std::string& name);
		VulkanPipelineBuilder& AddShader(const VulkanShaderModule& shaderModule);
		VulkanPipeline Build(ApplicationContext* context) const;
	};
}; // BetterThanNothing
