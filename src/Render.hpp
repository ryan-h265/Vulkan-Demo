#pragma once

#include <memory>        // std::shared_ptr
#include <unordered_set> // std::unordered_set
#include <vector>        // std::vector

#include <reactphysics3d/reactphysics3d.h>
#include <vulkan/vulkan.h>

#include "FPSCamera.hpp"
#include "Models/Model.hpp"
#include "Models/Rover.hpp"
#include "Shader.hpp"
#include "State.hpp"
#include "VulkanSetup.hpp"

class Render {
    Window& window;

  public:
    VkRenderPass renderPass;
    VulkanSetup vulkanSetup;
    Shader shader;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<Vertex> modelVertices;
    std::vector<uint32_t> modelIndices;

    std::vector<std::shared_ptr<Model>> objects;

    rp3d::PhysicsCommon physicsCommon;
    rp3d::PhysicsWorld* world;

    std::unordered_set<std::string> loadedModelClasses;

    gameState& state;

    Render(Window& window, gameState& state)
        : window(window), vulkanSetup(window, &renderPass),
          shader(&vulkanSetup.device, &renderPass), state(state) {}

    void initVulkan();
    void initImgui();

    void createCommandPool();
    void createRenderPass();
    void createSyncObjects();
    void createCommandBuffers();

  template <typename T>
  T addModel(glm::vec3 position,
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
         rp3d::BodyType bodyType = rp3d::BodyType::DYNAMIC);
    void drawFrame(FPSCamera::Matrices& matrices);
    void updateUniformBuffer(uint32_t currentImage,
                             FPSCamera::Matrices& matrices);
    void createPhysicsWorld();
    void createScene();
    void cleanup();

    void updateCharacterModelMatrix(glm::mat4 viewMatrix);

  private:
    std::vector<VkFence> inFlightFences;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    uint32_t currentFrame = 0;

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);
};
