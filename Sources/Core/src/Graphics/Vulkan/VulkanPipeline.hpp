//
// Created by acoezard on 11/19/24.
//

#pragma once

#include "VulkanShaderType.hpp"
#include "VulkanShaderModule.hpp"

namespace BetterThanNothing
{
	class ApplicationContext;

	struct VulkanShaderParams
	{
		VkShaderModule shaderModule;
		VulkanShaderType type;
	};

	struct VulkanPipelineParams
	{
		std::vector<VulkanShaderParams> shaders;
		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	};

	class VulkanPipeline
	{
	private:
		ApplicationContext* m_context;
		VulkanPipelineParams m_params;

	public:
		explicit VulkanPipeline(VulkanPipelineParams  params, ApplicationContext* context);
		~VulkanPipeline();

	private:
		static uint32_t _getShaderStageTypeByShaderType(VulkanShaderType type);
	};

	class VulkanPipelineBuilder
	{
	private:
		VulkanPipelineParams m_params{};

	public:
		explicit VulkanPipelineBuilder() = default;
		~VulkanPipelineBuilder() = default;

		VulkanPipelineBuilder& AddShader(const VulkanShaderModule& shaderModule, VulkanShaderType type);
		VulkanPipelineBuilder& SetInputAssembly(VkPrimitiveTopology topology);
		VulkanPipeline Build(ApplicationContext* context) const;
	};
}; // BetterThanNothing
