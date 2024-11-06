#pragma once

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	class Event;
	class KeyPressEvent;

	class Runtime: public Application {
	public:
		Runtime() = default;

		void OnEnable() override;
		void OnDisable() override;
	};
};
