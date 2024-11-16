#include "Application.hpp"

using namespace BetterThanNothing;

extern Application* CreateApplication();

int main() {
	auto pApplication = CreateApplication();

	try {
		pApplication->Run();
	} catch (const std::exception& e) {
		LOG_ERROR(e.what());
	}

	delete pApplication;

	return EXIT_SUCCESS;
}
