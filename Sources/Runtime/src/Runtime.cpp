#include "Runtime.hpp"

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	void Runtime::OnEnable(ApplicationContext* context)
	{
		context->GetRenderer()->AddPipeline([&](VulkanPipelineBuilder& builder) {
			builder.SetName("main");
			builder.AddVertexShader("main/main.vert");
			builder.AddFragmentShader("main/main.frag");
		});
	}

	void Runtime::OnUpdate(ApplicationContext* context)
	{
		(void) context;
	}

	void Runtime::OnRender(ApplicationContext* context)
	{
		(void) context;
	}

	void Runtime::OnDisable(ApplicationContext* context)
	{
		(void) context;
	}
};

BetterThanNothing::Application* CreateApplication()
{
	return new BetterThanNothing::Runtime();
}
