#pragma once

#include <iostream> // std::cout

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp> // glm::mat4

// I'm still undecided if this is the right place for this struct.
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class Shader {
    VkDevice* devicePtr;
    VkRenderPass* renderPassPtr;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

  public:
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    Shader(VkDevice* devicePtr, VkRenderPass* renderPassPtr)
        : devicePtr(devicePtr), renderPassPtr(renderPassPtr) {
        // print the render pass pointer
        std::cout << "Shader, devicePtr: " << devicePtr << std::endl;
        std::cout << "Shader, renderPassPtr: " << renderPassPtr << std::endl;
    }

    void loadShaders();
    void createGraphicsPipeline();
    void destroyGraphicsPipeline();
    void destroyPipelineLayout();
    void createDescriptorSetLayout();
    void cleanup();

  private:
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
};
