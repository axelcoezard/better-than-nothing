//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "BetterThanNothing.hpp"

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

		VulkanBufferingType bufferingType = VulkanBufferingType::None;
		bool enableVSync = true;
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

		std::unique_ptr<Renderer> m_pRenderer = nullptr;
		std::unique_ptr<ShaderPool> m_pShaderPool = nullptr;

	public:
		explicit ApplicationContext(ApplicationContextWindowParams  windowParams, ApplicationContextVulkanParams  vulkanParams);

		ApplicationContext& Initialize();

		std::unique_ptr<Window>& GetWindow();

		std::unique_ptr<VulkanInstance>& GetVulkanInstance();

		std::unique_ptr<VulkanSurface>& GetVulkanSurface();

		std::unique_ptr<VulkanDevice>& GetVulkanDevice();

		void SetGraphicsQueue(VkQueue graphicsQueue);

		std::unique_ptr<VulkanQueue>& GetGraphicsQueue();

		void SetPresentQueue(VkQueue presentQueue);

		std::unique_ptr<VulkanQueue>& GetPresentQueue();

		std::unique_ptr<VulkanMemoryAllocator>& GetVulkanMemoryAllocator();

		std::unique_ptr<Renderer>& GetRenderer();

		std::unique_ptr<ShaderPool>& GetShaderPool();

		VulkanShaderModule LoadShader(const std::string& name, VulkanShaderType type);

		[[nodiscard]]
		uint32_t GetVulkanApiVersion() const;

		void EnableValidationLayers(bool enable);

		[[nodiscard]]
		bool IsValidationLayersEnabled() const;

		[[nodiscard]]
		std::span<const char* const> GetValidationLayers() const;

		[[nodiscard]]
		std::span<const char* const> GetDeviceExtensions() const;

		[[nodiscard]]
		uint32_t GetMaxFrameInFlightCount() const;

		[[nodiscard]]
		bool IsVSyncEnabled() const;
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

		ApplicationContextBuilder& SetBufferingType(VulkanBufferingType type)
		{
			m_vulkanParams.bufferingType = type;
			return *this;
		}

		ApplicationContextBuilder& EnableVSync(bool enable)
		{
			m_vulkanParams.enableVSync = enable;
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

			if (m_vulkanParams.deviceExtensions.empty())
				throw ApplicationContextError("Device extensions are not set");

			if (m_vulkanParams.bufferingType == VulkanBufferingType::None)
				throw ApplicationContextError("Buffering type is not set");

			return ApplicationContext(m_windowParams, m_vulkanParams);
		}
	};
} // BetterThanNothing