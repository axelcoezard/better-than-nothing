//
// Created by acoezard on 11/5/24.
//

#include "BetterThanNothing.hpp"

namespace BetterThanNothing
{
	VulkanInstance::VulkanInstance(ApplicationContext* context): m_context(context)
	{
		bool validationLayersEnabled = context->IsValidationLayersEnabled();
		bool validationLayersAvalaible = _checkValidationLayerSupport();

		if (validationLayersEnabled && !validationLayersAvalaible)
			LOG_WARNING("Validation layers are enabled but not available on this system");

		validationLayersEnabled &= validationLayersAvalaible;
		context->EnableValidationLayers(validationLayersEnabled);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = context->GetVulkanApiVersion();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Prepare extensions
		const auto extensions = _getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Prepare validation layers
		if (validationLayersEnabled)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			_populateDebugMessengerCreateInfo(debugCreateInfo);

			const auto validationLayers = m_context->GetValidationLayers();

			createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		// Create instance
		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		LOG_SUCCESS("Vulkan instance: ok");

		_createDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		_destroyDebugMessenger();
		vkDestroyInstance(m_instance, nullptr);
	}

	std::vector<const char*> VulkanInstance::_getRequiredExtensions() noexcept
	{
		uint32 glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_context->IsValidationLayersEnabled()) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VulkanInstance::_checkValidationLayerSupport() noexcept
	{
		uint32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		const auto validationLayers = m_context->GetValidationLayers();

		return std::ranges::any_of(availableLayers, [validationLayers](const VkLayerProperties& layerProperties) {
			return std::ranges::any_of(validationLayers, [&layerProperties](const char* layerName) {
				return layerProperties.layerName == std::string_view(layerName);
			});
		});
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::s_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) noexcept
	{
		(void) messageType;
		(void) pUserData;

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			LOG_ERROR("validation layer: " << pCallbackData->pMessage);
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			LOG_WARNING("validation layer: " << pCallbackData->pMessage);
		}
		return VK_FALSE;
	}

	void VulkanInstance::_populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) noexcept
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = s_debugCallback;
		createInfo.pUserData = m_context;
	}

	void VulkanInstance::_createDebugMessenger()
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (!m_context->IsValidationLayersEnabled())
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		_populateDebugMessengerCreateInfo(createInfo);

		if (_createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}

		LOG_SUCCESS("Debug Messenger: ok");
	}

	void VulkanInstance::_destroyDebugMessenger()
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (!m_context->IsValidationLayersEnabled())
			return;

		if (m_debugMessenger != VK_NULL_HANDLE) {
			_destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}
	}

	VkResult VulkanInstance::_createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
			const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger) noexcept
	{
		auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (function == nullptr)
			return VK_ERROR_EXTENSION_NOT_PRESENT;

		return function(instance, createInfo, allocator, debugMessenger);
	}

	void VulkanInstance::_destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* allocator) noexcept
	{
		auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (function != nullptr) {
			function(instance, debugMessenger, allocator);
		}
	}

	VkInstance VulkanInstance::Handle() const
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");
		return m_instance;
	}
}


