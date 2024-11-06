//
// Created by acoezard on 11/5/24.
//

#include "VulkanInstance.hpp"
#include <stdexcept>

namespace BetterThanNothing
{
	VulkanInstance::VulkanInstance(const bool enableValidationLayers): m_enableValidationLayers(enableValidationLayers)
	{
		m_enableValidationLayers &= _checkValidationLayerSupport();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Prepare extensions
		const auto extensions = _getRequiredExtensions();
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Prepare validation layers
		if (m_enableValidationLayers)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			_populateDebugMessengerCreateInfo(debugCreateInfo);

			createInfo.enabledLayerCount = 1;
			createInfo.ppEnabledLayerNames = std::vector{ "VK_LAYER_KHRONOS_validation" }.data();
			createInfo.pNext = &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
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

		if (m_enableValidationLayers) {
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

		return std::ranges::any_of(availableLayers, [](const VkLayerProperties& layerProperties) {
			return std::string_view(layerProperties.layerName) == "VK_LAYER_KHRONOS_validation";
		});
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::s_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) noexcept
	{
		(void) messageType;
		(void) pUserData;

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
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

		if (!m_enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		_populateDebugMessengerCreateInfo(createInfo);

		if (_createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	void VulkanInstance::_destroyDebugMessenger()
	{
		if (m_instance == VK_NULL_HANDLE)
			throw std::runtime_error("Vulkan instance is not initialized");

		if (!m_enableValidationLayers)
			return;

		if (m_debugMessenger != VK_NULL_HANDLE) {
			_destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}
	}

	VkResult VulkanInstance::_createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) noexcept
	{
		auto function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
		);

		return function == nullptr ? VK_ERROR_EXTENSION_NOT_PRESENT : function(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	void VulkanInstance::_destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator) noexcept
	{
		auto function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
		);

		if (function != nullptr) {
			function(instance, debugMessenger, pAllocator);
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


