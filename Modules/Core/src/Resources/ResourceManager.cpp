#include "../BetterThanNothing.hpp"

namespace BetterThanNothing
{
	ResourceManager::ResourceManager(std::unique_ptr<Device>& device, const std::string& assetsPath)
	{
		m_ShaderPool = std::make_unique<ShaderPool>(assetsPath + "/Shaders/", device);
		m_ModelPool = std::make_unique<ModelPool>(assetsPath + "/Models/", device);
		m_TexturePool = std::make_unique<TexturePool>(assetsPath + "/Models/", device);
	}

	Shader* ResourceManager::GetShader(const std::string& path)
	{
		return m_ShaderPool->GetResource(path);
	}

	Model* ResourceManager::GetModel(const std::string& path)
	{
		return m_ModelPool->GetResource(path);
	}

	Texture* ResourceManager::GetTexture(const std::string& path)
	{
		return m_TexturePool->GetResource(path);
	}
};
