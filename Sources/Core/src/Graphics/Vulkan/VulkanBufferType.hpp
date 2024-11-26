//
// Created by acoezard on 11/25/24.
//

#pragma once

#include "vk_mem_alloc.h"

namespace BetterThanNothing
{
	enum class VulkanBufferType
	{
		STAGING_BUFFER = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VERTEX_BUFFER = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		INDEX_BUFFER = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	};
}