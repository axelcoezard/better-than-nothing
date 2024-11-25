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
		const char* name;
		std::vector<VulkanShaderModule> shadersInfos;
	};

	class VulkanPipeline
	{
	private:
		ApplicationContext* m_context = nullptr;
		VulkanPipelineParams m_params;

		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
		VkPipeline m_pipeline = VK_NULL_HANDLE;

	public:
		explicit VulkanPipeline(VulkanPipelineParams  params, ApplicationContext* context);
		~VulkanPipeline();

		[[nodiscard]]
		VkPipeline Handle() const;

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

		VulkanPipelineBuilder& SetName(const char* name);
		VulkanPipelineBuilder& AddShader(const std::string& shaderPath, VulkanShaderType shaderType);
		VulkanPipelineBuilder& AddVertexShader(const std::string& shaderPath);
		VulkanPipelineBuilder& AddFragmentShader(const std::string& shaderPath);

		[[nodiscard]]
		VulkanPipelineParams& GetBuildParams();
	};
}; // BetterThanNothing
