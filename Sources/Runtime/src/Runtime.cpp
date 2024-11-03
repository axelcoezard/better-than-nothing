#include "Runtime.hpp"

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	void Runtime::OnEnable()
	{

	}

	void Runtime::OnDisable()
	{

	}
};

BetterThanNothing::Application* CreateApplication()
{
	return new BetterThanNothing::Runtime();
}
