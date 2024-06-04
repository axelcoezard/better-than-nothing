#pragma once

namespace BetterThanNothing
{
	class Device;
	class ShaderPool;
	struct Shader;
	class ModelPool;
	struct Model;
	class TexturePool;
	struct Texture;

	class ResourceManager
	{
	private:
		ShaderPool* m_ShaderPool;
		ModelPool* m_ModelPool;
		TexturePool* m_TexturePool;

	public:
		ResourceManager(Device* device, const std::string& assetsPath);
		~ResourceManager();

		Shader* GetShader(const std::string& path);
		Model* GetModel(const std::string& path);
		Texture* GetTexture(const std::string& path);
	};
}
