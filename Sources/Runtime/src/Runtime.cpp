#include "Runtime.hpp"

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	void Runtime::OnEnable(ApplicationContext* context)
	{
		(void) context;
		auto vert = context->LoadShader("main/main.vert", VulkanShaderType::Vertex);
		auto frag = context->LoadShader("main/main.frag", VulkanShaderType::Fragment);

		VulkanPipeline pipeline = VulkanPipelineBuilder()
			.AddShader(vert)
			.AddShader(frag)
			.Build(context);
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
