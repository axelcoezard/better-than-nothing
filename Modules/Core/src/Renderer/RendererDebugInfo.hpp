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
		u32 sceneEntitiesCount;
	};
};
