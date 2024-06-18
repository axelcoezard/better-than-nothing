#pragma once

namespace BetterThanNothing
{
	struct RendererDebugInfo
	{
		std::string vendorName;
		std::string deviceName;
		std::string apiVersion;

		double frameCount;
		double deltaTime;

		std::string sceneName;
		uint32_t sceneEntitiesCount;

		uint32_t finishedJobsCount;
		uint32_t currentJobsCount;
		uint32_t waitingJobsCount;
	};
};
