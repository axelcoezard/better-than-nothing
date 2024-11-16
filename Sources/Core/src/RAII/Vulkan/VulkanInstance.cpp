//
// Created by acoezard on 11/5/24.
//

#include "VulkanInstance.hpp"
#include <stdexcept>

namespace BetterThanNothing
{
	VulkanInstance::VulkanInstance(const bool enableValidationLayers): m_validationLayersEnabled(enableValidationLayers)
	{
		m_validationLayersEnabled &= _checkValidationLayerSupport();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Prepare extensions
		const auto extensions = _getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Prepare validation layers
		if (m_validationLayersEnabled)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			_populateDebugMessengerCreateInfo(debugCreateInfo);

			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
			LOG_WARNING("Validation layers are not available on this system");
		}

		// Create instance
		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		LOG_INFO("Vulkan instance: ok");

		_createDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		_destroyDebugMessenger();
		vkDestroyInstance(m_instance, nullptr);
	}

	VulkanInstance::VulkanInstance(VulkanInstance&& other) noexcept
	{
		_move(std::move(other));
	}

	VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other) noexcept
	{
		if (this != &other)
			_move(std::move(other));
		return *this;
	}

	bool VulkanInstance::IsValidationLayersEnabled() const
	{
		return m_validationLayersEnabled;
	}

	std::vector<const char*> VulkanInstance::GetValidationLayers()
	{
		return m_validationLayers;
	}

	void VulkanInstance::_move(VulkanInstance&& other) noexcept
	{
		if (m_instance != VK_NULL_HANDLE)
		{
			_destroyDebugMessenger();
			vkDestroyInstance(m_instance, nullptr);
		}

		m_instance = other.m_instance;
		other.m_instance = VK_NULL_HANDLE;

		_createDebugMessenger();
	}

	std::vector<const char*> VulkanInstance::_getRequiredExtensions() noexcept
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_validationLayersEnabled) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VulkanInstance::_checkValidationLayerSupport() noexcept
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		return std::ranges::any_of(availableLayers, [this](const VkLayerProperties& layerProperties) {
			return std::ranges::any_of(m_validationLayers, [&layerProperties](const char* layerName) {
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
		createInfo.pUserData = nullptr; // TODO: Perhaps add a pointer to ApplicationContext
	}

	void VulkanInstance::_createDebugMessenger()
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (!m_validationLayersEnabled)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		_populateDebugMessengerCreateInfo(createInfo);

		if (_createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}

		LOG_INFO("Debug Messenger: ok");
	}

	void VulkanInstance::_destroyDebugMessenger()
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (!m_validationLayersEnabled)
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

	VulkanInstance::operator VkInstance() const
	{
		return m_instance;
	}

}


