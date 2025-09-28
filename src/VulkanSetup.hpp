#pragma once

#include <iostream>      // std::cout
#include <optional>      // std::optional
#include <unordered_map> // std::unordered_map

#include "Vertex.hpp"
#include "Window.hpp"

// #define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);

class VulkanSetup {
    Window& window;
    VkRenderPass* renderPassPtr;

  public:
    VkInstance instance;

    VulkanSetup(Window& window, VkRenderPass* renderPassPtr) : window(window) {
        this->renderPassPtr = renderPassPtr;

        // cout the render pass pointer
        std::cout << "VulkanSetup, renderPassPtr: " << renderPassPtr
                  << std::endl;
    }

    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;

    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    struct Image {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    };

    struct Texture {
        Image image;
        VkSampler sampler;
    };

    std::vector<Image> images;
    std::vector<Texture> textures;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    void recreateSwapChain(VkCommandPool* commandPoolPtr);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void cleanup();

    void setVertexBuffer(std::vector<Vertex> vertices,
                         VkCommandPool* commandPoolPtr,
                         bool createVertexBuffer = false);

    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createFramebuffers();
    void createVertexBuffer(VkCommandPool* commandPool,
                            std::vector<Vertex> vertices);
    void createIndexBuffer(VkCommandPool* commandPool,
                           std::vector<uint32_t> indices,
                           VkDeviceSize bufferSize);
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets(VkDescriptorSetLayout* descriptorSetLayoutPtr);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
                    VkCommandPool* commandPool);
    int createTexture(std::string texturePath, VkCommandPool* commandPoolPtr);
    void createTextureImage(VkCommandPool* commandPoolPtr,
                            VkImage* textureImagePtr,
                            VkDeviceMemory* textureImageMemoryPtr,
                            std::string texturePath);
    void createImage(uint32_t width, uint32_t height, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage& image,
                     VkDeviceMemory& imageMemory);
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool* commandPoolPtr);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer,
                               VkCommandPool* commandPoolPtr);
    void transitionImageLayout(VkImage image, VkFormat format,
                               VkImageLayout oldLayout, VkImageLayout newLayout,
                               VkCommandPool* commandPoolPtr);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height, VkCommandPool* commandPoolPtr);
    void createTextureImageView(VkImage textureImage,
                                VkImageView* textureImageViewPtr);
    void createTextureSampler(VkSampler* textureSamplerPtr);
    void createDepthResources(VkCommandPool* commandPoolPtr);
    VkFormat findDepthFormat();

  private:
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;

    std::vector<VkImage> swapChainImages;

    std::vector<VkImageView> swapChainImageViews;

    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData);
    int rateDeviceSuitability(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void cleanupSwapChain();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
};
