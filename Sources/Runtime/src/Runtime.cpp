#include "Runtime.hpp"

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	void Runtime::OnEnable(ApplicationContext* context)
	{
		context->GetRenderer()->AddPipeline([&](VulkanPipelineBuilder& builder) {
			builder.SetName("main");
			builder.AddShader("main/main.vert", VulkanShaderType::Vertex);
			builder.AddShader("main/main.frag", VulkanShaderType::Fragment);
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
