//
// Created by acoezard on 11/5/24.
//

#pragma once

#include <vector>

namespace BetterThanNothing
{
	class VulkanInstance
	{
		bool m_validationLayersEnabled = false;

		VkInstance m_instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

		const std::vector<const char*> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

	public:
		VulkanInstance() = default;
		explicit VulkanInstance(bool enableValidationLayers);
		~VulkanInstance();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		VulkanInstance(VulkanInstance&& other) noexcept;
		VulkanInstance& operator=(VulkanInstance&& other) noexcept;

		VkInstance Handle() const;
		explicit operator VkInstance() const;

		bool IsValidationLayersEnabled() const;
		std::vector<const char*> GetValidationLayers();

	private:
		void _move(VulkanInstance&& other) noexcept;
		bool _checkValidationLayerSupport() noexcept;
		std::vector<const char*> _getRequiredExtensions() noexcept;

		static VKAPI_ATTR VkBool32 VKAPI_CALL s_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) noexcept;

		void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) noexcept;
		void _createDebugMessenger();
		void _destroyDebugMessenger();

		VkResult _createDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger
		) noexcept;

		void _destroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator
		) noexcept;
	};
};


