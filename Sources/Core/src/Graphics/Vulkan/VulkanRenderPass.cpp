#include "VulkanRenderPass.hpp"
#include "ApplicationContext.hpp"

namespace BetterThanNothing
{
	VulkanRenderPass::VulkanRenderPass(ApplicationContext* context): m_context(context)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = context->GetVulkanSwapChain()->GetImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef; // Fragment shader out color should always be at location 0

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(context->GetVulkanDevice()->LogicalHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
			throw std::runtime_error("Failed to create render pass");

		LOG_SUCCESS("Vulkan render pass: ok");
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(m_context->GetVulkanDevice()->LogicalHandle(), m_renderPass, nullptr);
	}

	VkRenderPass VulkanRenderPass::Handle() const
	{
		return m_renderPass;
	}
}