#pragma once

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	class ApplicationContext;

	class Runtime: public Application {
	public:
		Runtime() = default;

		void OnEnable(ApplicationContext* context) override;
		void OnUpdate(ApplicationContext* context) override;
		void OnRender(ApplicationContext* context) override;
		void OnDisable(ApplicationContext* context) override;
	};
};
