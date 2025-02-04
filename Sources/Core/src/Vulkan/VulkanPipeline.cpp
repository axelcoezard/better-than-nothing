//
// Created by acoezard on 11/19/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanPipeline::VulkanPipeline(VulkanPipelineParams params, ApplicationContext* context)
		: m_context(context), m_params(std::move(params))
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (const auto& shaderInfos : m_params.shadersInfos)
		{
			auto shader = m_context->LoadShader(shaderInfos.shaderPath, shaderInfos.shaderType);

			VkShaderStageFlagBits shaderStageType = _getShaderStageTypeByShaderType(shader.shaderType);

			VkPipelineShaderStageCreateInfo shaderStageInfo{};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = shaderStageType;
			shaderStageInfo.module = shader.shaderModule;
			shaderStageInfo.pName = params.name;
			shaderStages.push_back(shaderStageInfo);
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		auto bindingDescription = Vertex::GetBindingDescription();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float32>(m_context->GetRenderer()->GetVulkanSwapChain()->GetExtent().width);
		viewport.height = static_cast<float32>(m_context->GetRenderer()->GetVulkanSwapChain()->GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_context->GetRenderer()->GetVulkanSwapChain()->GetExtent();

		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // use VK_POLYGON_MODE_LINE for wireframe
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(m_context->GetVulkanDevice()->LogicalHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline layout");

		LOG_SUCCESS("Vulkan pipeline layout: ok");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = m_context->GetRenderer()->GetVulkanRenderPass()->Handle();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(m_context->GetVulkanDevice()->LogicalHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
			throw std::runtime_error("Failed to create graphics pipeline");

		LOG_SUCCESS("Vulkan graphics pipeline: ok");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		if (m_pipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(m_context->GetVulkanDevice()->LogicalHandle(), m_pipelineLayout, nullptr);

		if (m_pipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(m_context->GetVulkanDevice()->LogicalHandle(), m_pipeline, nullptr);
	}

	VkPipeline VulkanPipeline::Handle() const
	{
		return m_pipeline;
	}

	VkShaderStageFlagBits VulkanPipeline::_getShaderStageTypeByShaderType(const VulkanShaderType type)
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

	VulkanPipelineBuilder& VulkanPipelineBuilder::SetName(const char* name)
	{
		m_params.name = name;
		return *this;
	}

	VulkanPipelineBuilder& VulkanPipelineBuilder::AddShader(const std::string& shaderPath, VulkanShaderType shaderType)
	{
		VulkanShaderModule shaderModule;
		shaderModule.shaderPath = shaderPath;
		shaderModule.shaderType = shaderType;

		m_params.shadersInfos.push_back(shaderModule);

		if (shaderType == VulkanShaderType::Vertex)
			m_hasVertexShader = true;

		if (shaderType == VulkanShaderType::Fragment)
			m_hasFragmentShader = true;

		return *this;
	}

	VulkanPipelineBuilder& VulkanPipelineBuilder::AddVertexShader(const std::string& shaderPath)
	{
		return AddShader(shaderPath, VulkanShaderType::Vertex);
	}

	VulkanPipelineBuilder& VulkanPipelineBuilder::AddFragmentShader(const std::string& shaderPath)
	{
		return AddShader(shaderPath, VulkanShaderType::Fragment);
	}

	VulkanPipelineParams& VulkanPipelineBuilder::GetBuildParams()
	{
		if (!m_hasVertexShader)
			throw std::runtime_error("Missing vertex shader");

		if (!m_hasFragmentShader)
			throw std::runtime_error("Missing fragment shader");

		return m_params;
	}
};