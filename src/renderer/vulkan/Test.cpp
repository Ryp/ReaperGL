////////////////////////////////////////////////////////////////////////////////
/// Reaper
///
/// Copyright (c) 2015-2020 Thibault Schueller
/// This file is distributed under the MIT License
////////////////////////////////////////////////////////////////////////////////

#include "Test.h"

#include "Debug.h"
#include "Memory.h"
#include "Swapchain.h"
#include "SwapchainRendererBase.h"

#include "api/VulkanStringConversion.h"

#include "renderpass/TestGraphics.h"

#include "renderer/texture/GPUTextureProperties.h"
#include "renderer/window/Event.h"
#include "renderer/window/Window.h"

#include "mesh/Mesh.h"
#include "mesh/ModelLoader.h"

#include "common/Log.h"
#include "common/ReaperRoot.h"

#include "core/BitTricks.h"
#include "core/Profile.h"
#include "core/memory/Allocator.h"

namespace Reaper
{
void debug_memory_heap_properties(ReaperRoot& root, const VulkanBackend& backend, uint32_t memoryTypeIndex)
{
    const VkMemoryType& memoryType = backend.physicalDeviceInfo.memory.memoryTypes[memoryTypeIndex];

    log_debug(root, "vulkan: selecting memory heap {} with these properties:", memoryType.heapIndex);
    for (u32 i = 0; i < sizeof(VkMemoryPropertyFlags) * 8; i++)
    {
        const VkMemoryPropertyFlags flag = 1 << i;

        if (memoryType.propertyFlags & flag)
            log_debug(root, "- {}", GetMemoryPropertyFlagBitToString(flag));
    }
}

void vulkan_test(ReaperRoot& root, VulkanBackend& backend)
{
    log_info(root, "test ////////////////////////////////////////");

    // Create descriptor pool
    constexpr u32                     MaxDescriptorSets = 100; // FIXME
    std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {{VK_DESCRIPTOR_TYPE_SAMPLER, 10},
                                                             {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 10},
                                                             {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10},
                                                             {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10},
                                                             {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 10}};

    VkDescriptorPoolCreateInfo poolInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                                           nullptr,
                                           0, // No special alloc flags
                                           MaxDescriptorSets,
                                           static_cast<uint32_t>(descriptorPoolSizes.size()),
                                           descriptorPoolSizes.data()};

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    Assert(vkCreateDescriptorPool(backend.device, &poolInfo, nullptr, &descriptorPool) == VK_SUCCESS);
    log_debug(root, "vulkan: created descriptor pool with handle: {}", static_cast<void*>(descriptorPool));

    // Create command buffer
    VkCommandPool           graphicsCommandPool = VK_NULL_HANDLE;
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr,
                                              VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                              backend.physicalDeviceInfo.graphicsQueueIndex};

    Assert(vkCreateCommandPool(backend.device, &poolCreateInfo, nullptr, &graphicsCommandPool) == VK_SUCCESS);
    log_debug(root, "vulkan: created command pool with handle: {}", static_cast<void*>(graphicsCommandPool));

    VkCommandBufferAllocateInfo cmdBufferAllocInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr,
                                                      graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1};

    VkCommandBuffer gfxCmdBuffer = VK_NULL_HANDLE;
    Assert(vkAllocateCommandBuffers(backend.device, &cmdBufferAllocInfo, &gfxCmdBuffer) == VK_SUCCESS);
    log_debug(root, "vulkan: created command buffer with handle: {}", static_cast<void*>(gfxCmdBuffer));

    {
        GlobalResources resources = {descriptorPool, gfxCmdBuffer};

        vulkan_test_graphics(root, backend, resources);
    }

    // cleanup
    vkFreeCommandBuffers(backend.device, graphicsCommandPool, 1, &gfxCmdBuffer);
    vkDestroyCommandPool(backend.device, graphicsCommandPool, nullptr);

    vkDestroyDescriptorPool(backend.device, descriptorPool, nullptr);

    log_info(root, "test ////////////////////////////////////////");
}
} // namespace Reaper
