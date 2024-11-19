#include "ApplicationContext.hpp"
#include "VulkanShaderType.hpp"
#include "VulkanShaderModule.hpp"

namespace BetterThanNothing
{
	VulkanShaderModule::VulkanShaderModule(const std::string& filename, VulkanShaderType shaderType, ApplicationContext* context)
	: m_context(context), m_filename(filename), m_shaderType(shaderType)
	{
		const std::vector<char> source = _readFile(filename);
		const auto stage = _getShaderStage(m_shaderType);
		const auto program = _getShaderProgram(source, stage);

		m_shaderModule = _getShaderModule(program);

		LOG_SUCCESS("Vulkan shader module '" << filename << "': ok");
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

	glslang_stage_t VulkanShaderModule::_getShaderStage(VulkanShaderType type)
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

	glslang_program_t* VulkanShaderModule::_getShaderProgram(const std::vector<char>& source, glslang_stage_t stage)
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

			spirv_cross::Compiler compiler(spirvCode, spirvSize);
			spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();

			LOG_INFO("Uniform buffers: " << shaderResources.uniform_buffers.size());
			LOG_INFO("Storage buffers: " << shaderResources.storage_buffers.size());
			LOG_INFO("Sampled images: " << shaderResources.sampled_images.size());
		}

		glslang_finalize_process();

		return program;
	}

	VkShaderModule VulkanShaderModule::_getShaderModule(glslang_program_t* program) const
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = glslang_program_SPIRV_get_size(program) * sizeof(unsigned int);
		createInfo.pCode = glslang_program_SPIRV_get_ptr(program);

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_context->GetVulkanDevice()->Handle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("failed to create shader module!");

		glslang_program_delete(program);

		return shaderModule;
	}

	VkShaderModule VulkanShaderModule::Handle() const
	{
		return m_shaderModule;
	}
};