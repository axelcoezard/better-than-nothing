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
	class AudioPool;
	struct Audio;

	class ResourceManager
	{
	private:
		std::unique_ptr<ShaderPool> m_ShaderPool;
		std::unique_ptr<ModelPool> m_ModelPool;
		std::unique_ptr<TexturePool> m_TexturePool;
		std::unique_ptr<AudioPool> m_AudioPool;

	public:
		ResourceManager(std::unique_ptr<Device>& device, const std::string& assetsPath);

		Shader* GetShader(const std::string& path);
		Model* GetModel(const std::string& path);
		Texture* GetTexture(const std::string& path);
		Audio* GetAudio(const std::string& path);
	};
}
