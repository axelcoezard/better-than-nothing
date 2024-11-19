//
// Created by acoezard on 11/19/24.
//

#include "ApplicationContext.hpp"
#include "VulkanPipeline.hpp"

#include <utility>

#include "VulkanShaderModule.hpp"

namespace BetterThanNothing
{
	VulkanPipeline::VulkanPipeline(VulkanPipelineParams  params, ApplicationContext* context)
		: m_context(context), m_params(std::move(params))
	{
		(void) m_context;
	}

	VulkanPipeline::~VulkanPipeline()
	{
	}

	uint32_t VulkanPipeline::_getShaderStageTypeByShaderType(const VulkanShaderType type)
	{
		switch (type)
		{
		case VulkanShaderType::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case VulkanShaderType::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		default:
			throw std::runtime_error("Invalid shader type");
		}
	}

	VulkanPipelineBuilder& VulkanPipelineBuilder::AddShader(const VulkanShaderModule& shaderModule, const VulkanShaderType type)
	{
		m_params.shaders.push_back({
			.shaderModule = shaderModule.Handle(),
			.type = type
		});
		return *this;
	}

	VulkanPipelineBuilder& VulkanPipelineBuilder::SetInputAssembly(const VkPrimitiveTopology topology)
	{
		m_params.topology = topology;
		return *this;
	}

	VulkanPipeline VulkanPipelineBuilder::Build(ApplicationContext* context) const
	{
		return VulkanPipeline(m_params, context);
	}
};