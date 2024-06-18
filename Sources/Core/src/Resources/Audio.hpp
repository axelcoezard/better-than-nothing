#pragma once

#include <string>
#include <vector>

namespace BetterThanNothing
{
	struct Audio
	{
		std::string filePath;

		ALuint buffer;
		ALint channels;
		ALint frames;
		ALenum format;
		ALsizei sampleRate;
		ALsizei sampleCount;

		std::vector<ALshort> samples;
	};
}
