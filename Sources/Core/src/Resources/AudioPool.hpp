#pragma once

#include <memory>
#include "ResourcePool.hpp"
#include "Audio.hpp"

namespace BetterThanNothing
{
	class Device;

	class AudioPool: public ResourcePool<Audio>
	{
	private:
		std::unique_ptr<Device>& m_Device;

	public:
		AudioPool(const std::string& basePath, std::unique_ptr<Device>& device);
		~AudioPool() override;

		Audio* GetResource(const std::string& filePath) override;
	};
};
