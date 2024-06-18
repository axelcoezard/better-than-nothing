#include "../BetterThanNothing.hpp"

#include "../../../../ThirdParty/stb/include/stb_image.h"

namespace BetterThanNothing
{
	AudioPool::AudioPool(const std::string& basePath, std::unique_ptr<Device>& device)
		: ResourcePool(basePath), m_Device(device)
	{
		(void) m_Device;
	}

	AudioPool::~AudioPool()
	{
		for (auto& resource : m_Resources)
		{
			alDeleteBuffers(1, &resource.second->buffer);
			delete resource.second;
		}
	}

	Audio* AudioPool::GetResource(const std::string& filePath)
	{
		auto it = m_Resources.find(filePath);
		if (it != m_Resources.end())
			return it->second;

		auto* audio = new Audio();

		SF_INFO audioInfo;
		SNDFILE* audioFile = sf_open(std::string(m_BasePath + filePath).c_str(), SFM_READ, &audioInfo);
		// TODO: Check if audio is valid

		audio->filePath = filePath;
		audio->channels = audioInfo.channels;
		audio->frames = audioInfo.frames;
		audio->sampleRate = audioInfo.samplerate;
		audio->sampleCount = audio->channels * audio->frames;
		audio->samples.resize(audio->sampleCount);

		sf_read_short(
			audioFile,
			audio->samples.data(),
			static_cast<int>(audio->samples.size())
		);
		// TODO: Check if samples are valid

		sf_close(audioFile);

		switch (audioInfo.channels)
		{
			case 1 : audio->format = AL_FORMAT_MONO16; break;
			case 2 : audio->format = AL_FORMAT_STEREO16; break;
		}

		alGenBuffers(1, &audio->buffer);
		alBufferData(
			audio->buffer,
			audio->format,
			&audio->samples.at(0),
			audio->sampleCount * sizeof(ALushort),
			audio->sampleRate
		);
		// TODO: Check if buffer is valid

		LOG_SUCCESS("AudioPool: " + filePath);

		m_Resources[filePath] = audio;
		return audio;
	}
};
