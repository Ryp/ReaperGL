////////////////////////////////////////////////////////////////////////////////
/// ReaperGL
///
/// Copyright (c) 2015-2016 Thibault Schueller
/// This file is distributed under the MIT License
////////////////////////////////////////////////////////////////////////////////

#ifndef REAPER_VULKANRENDERER_INCLUDED
#define REAPER_VULKANRENDERER_INCLUDED

#include <vector>

#include "renderer/Renderer.h"
#include "api/Vulkan.h"
#include "core/DynamicLibrary.h"


struct VSUBO;

class VulkanRenderer : public AbstractRenderer
{
public:
    VulkanRenderer() = default;
    ~VulkanRenderer() = default;

public:
    void startup(Window* window) override;
    void shutdown() override;
    void render() override;

private:
    void createSemaphores();
    void createSwapChain();
    void createCommandBuffers();
    void createDescriptorPool();
    void createRenderPass();
    void createFramebuffers();
    void createPipeline();
    void createMeshBuffers();
    void createDescriptorSet();

private:
    void updateUniforms();

private:
    LibHandle           _vulkanLib;

private:
    VkInstance          _instance;
    VkPhysicalDevice    _physicalDevice;
    VkSurfaceKHR        _presentationSurface;
    VkQueue             _presentationQueue;
    VkQueue             _graphicsQueue;
    VkSwapchainKHR      _swapChain;
    std::vector<VkImage>_swapChainImages;
    VkFormat            _swapChainFormat;
    VkDevice            _device;
    VkPipeline          _pipeline;
    VkPipelineLayout    _pipelineLayout;
    VkSemaphore         _imageAvailableSemaphore;
    VkSemaphore         _renderingFinishedSemaphore;

private:
    VkDebugReportCallbackEXT _debugCallback;

private:
    VkDeviceMemory  _deviceMemory;
    VkBuffer        _vertexBuffer;
    VkBuffer        _indexBuffer;

private:
    VkCommandPool                   _gfxCmdPool;
    std::vector<VkCommandBuffer>    _gfxCmdBuffers;
    uint32_t                        _gfxQueueIndex;
    uint32_t                        _prsQueueIndex;

private:
    VkDescriptorPool        _descriptorPool;
    VkDescriptorSet         _descriptorSet;
    VkDescriptorSetLayout   _descriptorSetLayout;

private:
    struct {
        VkBuffer buffer;
        VkDeviceMemory memory;
        VkDescriptorBufferInfo descriptor;
    }  _uniformData;

private:
    VkRenderPass                _renderPass;
    std::vector<VkFramebuffer>  _framebuffers;
    std::vector<VkImageView>    _swapChainImageViews;

private:
    VSUBO* _uniforms;
};

#endif // REAPER_VULKANRENDERER_INCLUDED
