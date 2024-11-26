//
// Created by acoezard on 11/5/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	ApplicationContext::ApplicationContext(ApplicationContextWindowParams windowParams, ApplicationContextVulkanParams vulkanParams)
		: m_windowParams(std::move(windowParams)), m_vulkanParams(std::move(vulkanParams))
	{
	}

	ApplicationContext& ApplicationContext::Initialize()
	{
		m_pWindow = std::make_unique<Window>(
			m_windowParams.title,
			m_windowParams.width,
			m_windowParams.height,
			m_windowParams.fullscreen,
			m_windowParams.resizable
		);

		m_pVulkanInstance = std::make_unique<VulkanInstance>(this);
		m_pVulkanSurface = std::make_unique<VulkanSurface>(this);
		m_pVulkanDevice = std::make_unique<VulkanDevice>(this);

		LOG_INFO("Vendor: " << m_pVulkanDevice->GetVendorName());
		LOG_INFO("Device: " << m_pVulkanDevice->GetDeviceName());
		LOG_INFO("API Version: " << m_pVulkanDevice->GetApiVersion());

		m_pVulkanMemoryAllocator = std::make_unique<VulkanMemoryAllocator>(this);

		LOG_INFO("Buffering mode: " << GetMaxFrameInFlightCount());
		LOG_INFO("VSync: " << (m_vulkanParams.enableVSync ? "enabled" : "disabled"));

		m_pRenderer = std::make_unique<VulkanRenderer>(this);
		m_pRenderer->Initialize();

		m_pShaderPool = std::make_unique<ShaderPool>(m_vulkanParams.shadersFolderPath, this);

		return *this;
	}

	std::unique_ptr<Window>& ApplicationContext::GetWindow()
	{
		if (!m_pWindow)
			throw ApplicationContextError("Window is not set");
		return m_pWindow;
	}

	std::unique_ptr<VulkanInstance>& ApplicationContext::GetVulkanInstance()
	{
		if (!m_pVulkanInstance)
			throw ApplicationContextError("Vulkan instance is not set");
		return m_pVulkanInstance;
	}

	std::unique_ptr<VulkanSurface>& ApplicationContext::GetVulkanSurface()
	{
		if (!m_pVulkanSurface)
			throw ApplicationContextError("Vulkan surface is not set");
		return m_pVulkanSurface;
	}

	std::unique_ptr<VulkanDevice>& ApplicationContext::GetVulkanDevice()
	{
		if (!m_pVulkanDevice)
			throw ApplicationContextError("Vulkan device is not set");
		return m_pVulkanDevice;
	}

	void ApplicationContext::SetGraphicsQueue(VkQueue graphicsQueue)
	{
		m_pGraphicsQueue = std::make_unique<VulkanQueue>(graphicsQueue);
	}

	std::unique_ptr<VulkanQueue>& ApplicationContext::GetGraphicsQueue()
	{
		if (!m_pGraphicsQueue)
			throw ApplicationContextError("Graphics queue is not set");
		return m_pGraphicsQueue;
	}

	void ApplicationContext::SetPresentQueue(VkQueue presentQueue)
	{
		m_pPresentQueue = std::make_unique<VulkanQueue>(presentQueue);
	}

	std::unique_ptr<VulkanQueue>& ApplicationContext::GetPresentQueue()
	{
		if (!m_pPresentQueue)
			throw ApplicationContextError("Presentation queue is not set");
		return m_pPresentQueue;
	}

	std::unique_ptr<VulkanMemoryAllocator>& ApplicationContext::GetVulkanMemoryAllocator()
	{
		if (!m_pVulkanMemoryAllocator)
			throw ApplicationContextError("Vulkan memory allocator is not set");
		return m_pVulkanMemoryAllocator;
	}

	std::unique_ptr<VulkanRenderer>& ApplicationContext::GetRenderer()
	{
		if (!m_pRenderer)
			throw ApplicationContextError("Renderer is not set");
		return m_pRenderer;
	}

	std::unique_ptr<ShaderPool>& ApplicationContext::GetShaderPool()
	{
		if (!m_pShaderPool)
			throw ApplicationContextError("Shader pool is not set");
		return m_pShaderPool;
	}

	VulkanShaderModule ApplicationContext::LoadShader(const std::string& name, VulkanShaderType type)
	{
		return m_pShaderPool->LoadShader(name, type);
	}

	[[nodiscard]]
	uint32 ApplicationContext::GetVulkanApiVersion() const
	{
		return m_vulkanParams.apiVersion;
	}

	void ApplicationContext::EnableValidationLayers(const bool enable)
	{
		m_vulkanParams.enableValidationLayers = enable;
	}

	[[nodiscard]]
	bool ApplicationContext::IsValidationLayersEnabled() const
	{
		return m_vulkanParams.enableValidationLayers;
	}

	[[nodiscard]]
	std::span<const char* const> ApplicationContext::GetValidationLayers() const
	{
		return m_vulkanParams.validationLayers;
	}

	[[nodiscard]]
	std::span<const char* const> ApplicationContext::GetDeviceExtensions() const
	{
		return m_vulkanParams.deviceExtensions;
	}

	[[nodiscard]]
	uint32 ApplicationContext::GetMaxFrameInFlightCount() const
	{
		return static_cast<uint32>(m_vulkanParams.bufferingMode);
	}

	[[nodiscard]]
	bool ApplicationContext::IsVSyncEnabled() const
	{
		return m_vulkanParams.enableVSync;
	}
} // BetterThanNothing