//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "Graphics/Window.hpp"
#include "Graphics/Vulkan/VulkanInstance.hpp"
#include "Graphics/Vulkan/VulkanSurface.hpp"
#include "Graphics/Vulkan/VulkanDevice.hpp"
#include "Graphics/Vulkan/VulkanQueue.hpp"
#include "Graphics/Vulkan/VulkanMemoryAllocator.hpp"
#include "Graphics/Vulkan/VulkanSwapChain.hpp"
#include "Graphics/Vulkan/VulkanShaderType.hpp"
#include "Graphics/Vulkan/VulkanShaderModule.hpp"
#include "Graphics/ShaderPool.hpp"
#include "Graphics/Vulkan/VulkanRenderPass.hpp"
#include "Graphics/Vulkan/VulkanFramebuffer.hpp"
#include "Graphics/Vulkan/VulkanCommandPool.hpp"

namespace BetterThanNothing
{
	class ApplicationContextError : public std::runtime_error
	{
	public:
		explicit ApplicationContextError(const std::string& message)
			: std::runtime_error(message) {}
	};

	struct ApplicationContextWindowParams
	{
		std::string title;
		uint32_t width = 0;
		uint32_t height = 0;
		bool fullscreen = false;
		bool resizable = false;
	};

	struct ApplicationContextVulkanParams
	{
		uint32_t apiVersion = VK_API_VERSION_1_3;

		bool enableValidationLayers = false;
		std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		std::string shadersFolderPath;
	};

	class ApplicationContext
	{
	private:
		ApplicationContextWindowParams m_windowParams{};
		ApplicationContextVulkanParams m_vulkanParams{};

		std::unique_ptr<Window> m_pWindow = nullptr;

		std::unique_ptr<VulkanInstance> m_pVulkanInstance = nullptr;
		std::unique_ptr<VulkanSurface> m_pVulkanSurface = nullptr;
		std::unique_ptr<VulkanDevice> m_pVulkanDevice = nullptr;

		std::unique_ptr<VulkanQueue> m_pGraphicsQueue = nullptr;
		std::unique_ptr<VulkanQueue> m_pPresentQueue = nullptr;

		std::unique_ptr<VulkanMemoryAllocator> m_pVulkanMemoryAllocator = nullptr;
		std::unique_ptr<VulkanCommandPool> m_pVulkanCommandPool = nullptr;

		std::unique_ptr<VulkanSwapChain> m_pVulkanSwapChain = nullptr;

		std::unique_ptr<ShaderPool> m_pShaderPool = nullptr;

		std::unique_ptr<VulkanRenderPass> m_pVulkanRenderPass = nullptr;

	public:
		explicit ApplicationContext(ApplicationContextWindowParams  windowParams, ApplicationContextVulkanParams  vulkanParams)
			: m_windowParams(std::move(windowParams)), m_vulkanParams(std::move(vulkanParams)) {}

		~ApplicationContext() = default;

		ApplicationContext& Initialize()
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

			m_pVulkanMemoryAllocator = std::make_unique<VulkanMemoryAllocator>(this);
			m_pVulkanCommandPool = std::make_unique<VulkanCommandPool>(this);

			m_pVulkanSwapChain = std::make_unique<VulkanSwapChain>(this);
			m_pVulkanSwapChain->CreateImages();
			m_pVulkanSwapChain->CreateImageViews();

			m_pShaderPool = std::make_unique<ShaderPool>(m_vulkanParams.shadersFolderPath, this);
			m_pVulkanRenderPass = std::make_unique<VulkanRenderPass>(this);

			m_pVulkanSwapChain->CreateFramebuffers();

			return *this;
		}

		std::unique_ptr<Window>& GetWindow()
		{
			if (!m_pWindow)
				throw ApplicationContextError("Window is not set");
			return m_pWindow;
		}

		std::unique_ptr<VulkanInstance>& GetVulkanInstance()
		{
			if (!m_pVulkanInstance)
				throw ApplicationContextError("Vulkan instance is not set");
			return m_pVulkanInstance;
		}

		std::unique_ptr<VulkanSurface>& GetVulkanSurface()
		{
			if (!m_pVulkanSurface)
				throw ApplicationContextError("Vulkan surface is not set");
			return m_pVulkanSurface;
		}

		std::unique_ptr<VulkanDevice>& GetVulkanDevice()
		{
			if (!m_pVulkanDevice)
				throw ApplicationContextError("Vulkan device is not set");
			return m_pVulkanDevice;
		}

		void SetGraphicsQueue(VkQueue graphicsQueue)
		{
			m_pGraphicsQueue = std::make_unique<VulkanQueue>(graphicsQueue);
		}

		std::unique_ptr<VulkanQueue>& GetGraphicsQueue()
		{
			if (!m_pGraphicsQueue)
				throw ApplicationContextError("Graphics queue is not set");
			return m_pGraphicsQueue;
		}

		void SetPresentQueue(VkQueue presentQueue)
		{
			m_pPresentQueue = std::make_unique<VulkanQueue>(presentQueue);
		}

		std::unique_ptr<VulkanQueue>& GetPresentQueue()
		{
			if (!m_pPresentQueue)
				throw ApplicationContextError("Presentation queue is not set");
			return m_pPresentQueue;
		}

		std::unique_ptr<VulkanMemoryAllocator>& GetVulkanMemoryAllocator()
		{
			if (!m_pVulkanMemoryAllocator)
				throw ApplicationContextError("Vulkan memory allocator is not set");
			return m_pVulkanMemoryAllocator;
		}

		std::unique_ptr<VulkanCommandPool>& GetVulkanCommandPool()
		{
			if (!m_pVulkanCommandPool)
				throw ApplicationContextError("Vulkan command pool is not set");
			return m_pVulkanCommandPool;
		}

		std::unique_ptr<VulkanSwapChain>& GetVulkanSwapChain()
		{
			if (!m_pVulkanSwapChain)
				throw ApplicationContextError("Vulkan swap chain is not set");
			return m_pVulkanSwapChain;
		}

		std::unique_ptr<ShaderPool>& GetShaderPool()
		{
			if (!m_pShaderPool)
				throw ApplicationContextError("Shader pool is not set");
			return m_pShaderPool;
		}

		std::unique_ptr<VulkanRenderPass>& GetVulkanRenderPass()
		{
			if (!m_pVulkanRenderPass)
				throw ApplicationContextError("Vulkan render pass is not set");
			return m_pVulkanRenderPass;
		}

		VulkanShaderModule LoadShader(const std::string& name, VulkanShaderType type)
		{
			return m_pShaderPool->LoadShader(name, type);
		}

		[[nodiscard]]
		uint32_t GetVulkanApiVersion() const
		{
			return m_vulkanParams.apiVersion;
		}

		void EnableValidationLayers(const bool enable)
		{
			m_vulkanParams.enableValidationLayers = enable;
		}

		[[nodiscard]]
		bool IsValidationLayersEnabled() const
		{
			return m_vulkanParams.enableValidationLayers;
		}

		[[nodiscard]]
		std::span<const char* const> GetValidationLayers() const
		{
			return m_vulkanParams.validationLayers;
		}

		[[nodiscard]]
		std::span<const char* const> GetDeviceExtensions() const
		{
			return m_vulkanParams.deviceExtensions;
		}
	};

	class ApplicationContextBuilder
	{
	private:
		ApplicationContextWindowParams m_windowParams{};
		ApplicationContextVulkanParams m_vulkanParams{};

	public:
		ApplicationContextBuilder() = default;
		~ApplicationContextBuilder() = default;

		ApplicationContextBuilder& SetWindowTitle(const std::string& title)
		{
			m_windowParams.title = title;
			return *this;
		}

		ApplicationContextBuilder& SetWindowSize(uint32_t width, uint32_t height)
		{
			m_windowParams.width = width;
			m_windowParams.height = height;
			return *this;
		}

		ApplicationContextBuilder& SetWindowFullscreen(bool fullscreen)
		{
			m_windowParams.fullscreen = fullscreen;
			return *this;
		}

		ApplicationContextBuilder& SetWindowResizable(bool resizable)
		{
			m_windowParams.resizable = resizable;
			return *this;
		}

		ApplicationContextBuilder& SetVulkanApiVersion(uint32_t version)
		{
			m_vulkanParams.apiVersion = version;
			return *this;
		}

		ApplicationContextBuilder& EnableValidationLayers(bool enable)
		{
			m_vulkanParams.enableValidationLayers = enable;
			return *this;
		}

		ApplicationContextBuilder& AddValidationLayer(const char* layer)
		{
			m_vulkanParams.validationLayers.push_back(layer);
			return *this;
		}

		ApplicationContextBuilder& RequireDeviceExtension(const char* extension)
		{
			m_vulkanParams.deviceExtensions.push_back(extension);
			return *this;
		}

		ApplicationContextBuilder& SetShadersFolderPath(const std::string& path)
		{
			m_vulkanParams.shadersFolderPath = path;
			return *this;
		}

		ApplicationContext Build()
		{
			if (m_windowParams.title.empty())
				throw ApplicationContextError("Window title is not set");

			if (m_windowParams.width == 0 || m_windowParams.height == 0)
				throw ApplicationContextError("Window size is not set");

			if (m_vulkanParams.enableValidationLayers && m_vulkanParams.validationLayers.empty())
				throw ApplicationContextError("Validation layers are enabled but none are set");

			if (m_vulkanParams.shadersFolderPath.empty())
				throw ApplicationContextError("Assets folder path is not set");

			return ApplicationContext(m_windowParams, m_vulkanParams);
		}
	};
} // BetterThanNothing