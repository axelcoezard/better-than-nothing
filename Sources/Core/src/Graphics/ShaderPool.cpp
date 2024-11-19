//
// Created by acoezard on 11/19/24.
//

#include "ShaderPool.hpp"

#include "ApplicationContext.hpp"
#include "Vulkan/VulkanShaderType.hpp"
#include "Vulkan/VulkanShaderModule.hpp"

#include <utility>

namespace BetterThanNothing
{
	ShaderPool::~ShaderPool()
	{
		auto device = m_context->GetVulkanDevice()->LogicalHandle();
		for (const auto& shader : m_shaders)
			vkDestroyShaderModule(device, shader.second.shaderModule, nullptr);
	}

	VulkanShaderModule ShaderPool::LoadShader(const std::string& filename, const VulkanShaderType type)
	{
		auto it = m_shaders.find(filename);
		if (it != m_shaders.end())
			return it->second;

		std::vector<char> shaderSource = _readFile(m_basePath + filename);
		glslang_stage_t shaderStage = _getShaderStage(type);
		glslang_program_t* shaderProgram = _getShaderProgram(shaderSource, shaderStage);

		m_shaders[filename] = {
			.filename = filename,
			.shaderType = type,
			.shaderModule = _getShaderModule(shaderProgram)
		};

		LOG_SUCCESS("Shader loaded: " << filename);

		return m_shaders[filename];
	}

	std::vector<char> ShaderPool::_readFile(const std::string& filename)
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

	glslang_stage_t ShaderPool::_getShaderStage(VulkanShaderType type)
	{
		switch (type)
		{
			case VulkanShaderType::Vertex:
				return GLSLANG_STAGE_VERTEX;
			case VulkanShaderType::Fragment:
				return GLSLANG_STAGE_FRAGMENT;
			default:
				throw std::runtime_error("Invalid shader type");
		}
	}

	glslang_program_t* ShaderPool::_getShaderProgram(const std::vector<char>& source, glslang_stage_t stage)
	{
		const glslang_input_t input = {
			.language = GLSLANG_SOURCE_GLSL,
			.stage = stage,
			.client = GLSLANG_CLIENT_VULKAN,
			.client_version = GLSLANG_TARGET_VULKAN_1_3,
			.target_language = GLSLANG_TARGET_SPV,
			.target_language_version = GLSLANG_TARGET_SPV_1_0,
			.code = source.data(),
			.default_version = 450,
			.default_profile = GLSLANG_NO_PROFILE,
			.force_default_version_and_profile = false,
			.forward_compatible = false,
			.messages = GLSLANG_MSG_DEFAULT_BIT,
			.resource = glslang_default_resource(),
			.callbacks = {},
			.callbacks_ctx = nullptr
		};

		glslang_initialize_process();

		glslang_shader_t* shader = glslang_shader_create(&input);

		if (!glslang_shader_preprocess(shader, &input))
			throw std::runtime_error("failed to preprocess shader: " + std::string(glslang_shader_get_info_log(shader)));

		if (!glslang_shader_parse(shader, &input))
			throw std::runtime_error("failed to parse shader: " + std::string(glslang_shader_get_info_log(shader)));

		glslang_program_t* program = glslang_program_create();
		glslang_program_add_shader(program, shader);

		if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
			throw std::runtime_error("failed to compile shader program: " + std::string(glslang_program_get_info_log(program)));

		glslang_program_SPIRV_generate(program, input.stage);

		if (glslang_program_SPIRV_get_messages(program))
			LOG_INFO("Shader messages: " << glslang_program_SPIRV_get_messages(program));

		glslang_shader_delete(shader);

		{
			const uint32_t* spirvCode = glslang_program_SPIRV_get_ptr(program);
			const size_t spirvSize = glslang_program_SPIRV_get_size(program);

			const spirv_cross::Compiler compiler(spirvCode, spirvSize);
			const spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();

			if (!shaderResources.stage_inputs.empty())
			{
				for (const auto& resource : shaderResources.stage_inputs)
					LOG_INFO("Stage input: " << resource.name);
			}

			if (!shaderResources.stage_outputs.empty())
			{
				for (const auto& resource : shaderResources.stage_outputs)
					LOG_INFO("Stage output: " << resource.name);
			}

			if (!shaderResources.uniform_buffers.empty())
				LOG_INFO("Uniform buffers: " << shaderResources.uniform_buffers.size());

			if (!shaderResources.storage_buffers.empty())
				LOG_INFO("Storage buffers: " << shaderResources.storage_buffers.size());

			if (!shaderResources.sampled_images.empty())
				LOG_INFO("Sampled images: " << shaderResources.sampled_images.size());
		}

		glslang_finalize_process();

		return program;
	}

	VkShaderModule ShaderPool::_getShaderModule(glslang_program_t* program) const
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = glslang_program_SPIRV_get_size(program) * sizeof(unsigned int);
		createInfo.pCode = glslang_program_SPIRV_get_ptr(program);

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_context->GetVulkanDevice()->LogicalHandle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("failed to create shader module!");

		glslang_program_delete(program);

		return shaderModule;
	}

};
