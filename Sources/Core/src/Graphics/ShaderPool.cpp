//
// Created by acoezard on 11/19/24.
//

#include "ApplicationContext.hpp"
#include "Vulkan/VulkanShaderModule.hpp"
#include "ShaderPool.hpp"

#include <utility>

namespace BetterThanNothing
{
	ShaderPool::ShaderPool(std::string  basePath, ApplicationContext* context): m_context(context), m_basePath(std::move(basePath))
	{
	}

	VkShaderModule ShaderPool::LoadShader(const std::string& filename, const VulkanShaderType type)
	{
		auto it = m_shaders.find(filename);
		if (it != m_shaders.end())
			return it->second->Handle();

		m_shaders[filename] = std::make_unique<VulkanShaderModule>(m_basePath + filename, type, m_context);

		return m_shaders[filename]->Handle();
	}

};
