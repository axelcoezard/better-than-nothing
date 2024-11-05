//
// Created by acoezard on 11/5/24.
//

#pragma once

#include "VkBootstrap.h"

namespace BetterThanNothing
{
	class VulkanInstance
	{
		vkb::Instance m_vkbInstance;

	public:
		explicit VulkanInstance(bool enableValidationLayers = false);
		~VulkanInstance();

		// Prevent copy to ensure unique ownership of nodes
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		// Allow move semantics
		VulkanInstance(VulkanInstance&& other) noexcept;
		VulkanInstance& operator=(VulkanInstance&& other) noexcept;

		VkInstance Handle() const;
		explicit operator VkInstance() const;

	private:
		void _swap(VulkanInstance&& other);
	};
};


