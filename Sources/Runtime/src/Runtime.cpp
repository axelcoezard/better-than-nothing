#include "Runtime.hpp"

#include <BetterThanNothing.hpp>

namespace BetterThanNothing
{
	void Runtime::OnEnable(ApplicationContext* context)
	{
		VulkanPipeline pipeline = VulkanPipelineBuilder()
			.SetName("main")
			.AddShader("main/main.vert", VulkanShaderType::Vertex)
			.AddShader("main/main.frag", VulkanShaderType::Fragment)
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
