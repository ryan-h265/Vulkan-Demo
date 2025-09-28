#include <iostream> // std::cout

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

#include "imgui/imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <chrono>  // std::chrono
#include <cstring> // memcpy

#include "FPSCamera.hpp"
#include "Models/Box.hpp"
#include "Models/Bridge.hpp"
#include "Models/Commodore.hpp"
#include "Models/Hatchet.hpp"
#include "Models/House.hpp"
#include "Models/Skull.hpp"
#include "Render.hpp"

void Render::createScene() {
    // floor
    addModel<Box>(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(20.0f, 0.5f, 20.0f),
                rp3d::BodyType::STATIC);

    // Move the brick structure further away (z = -10), extend the walls, and add a fourth wall
    glm::vec3 brickSize = glm::vec3(0.5f, 0.25f, 0.5f);

    float wallHeight = 7.0f;
    float wallStep = 0.5f;
    float wallLength = 20.0f; // longer walls
    float wallZStart = -15.0f;
    float wallZEnd = 0.0f;
    float wallXStart = -10.0f;
    float wallXEnd = 10.0f;
    float structureZ = -10.0f; // further away

    // Front wall (X direction)
    bool step = true;
    for (float y = 0; y < wallHeight; y += wallStep) {
        for (float x = wallXStart; x < wallXEnd; x++) {
            float xOffset = step ? 0.5f : 0.0f;
            addModel<Box>(glm::vec3(x + xOffset, y, structureZ + 10.0f), brickSize);
        }
        step = !step;
    }

    // Side wall 1 (Z direction, left)
    glm::vec3 brickSize2 = glm::vec3(0.5f, 0.25f, 0.5f);
    step = false;
    for (float y = 0; y < wallHeight; y += wallStep) {
        for (float z = wallZStart; z < wallZEnd; z++) {
            float zOffset = step ? 0.5f : 0.0f;
            addModel<Box>(glm::vec3(wallXStart - 0.25f, y, z + zOffset + 0.25f + structureZ + 10.0f), brickSize2);
        }
        step = !step;
    }

    // Side wall 2 (Z direction, right)
    step = true;
    for (float y = 0; y < wallHeight; y += wallStep) {
        for (float z = wallZStart; z < wallZEnd; z++) {
            float zOffset = step ? 0.5f : 0.0f;
            addModel<Box>(glm::vec3(wallXEnd - 0.25f, y, z + zOffset + 0.25f + structureZ + 10.0f), brickSize2);
        }
        step = !step;
    }

    // Back wall (X direction, at far z)
    step = false;
    for (float y = 0; y < wallHeight; y += wallStep) {
        for (float x = wallXStart; x < wallXEnd; x++) {
            float xOffset = step ? 0.5f : 0.0f;
            addModel<Box>(glm::vec3(x + xOffset, y, wallZStart + structureZ + 10.0f), brickSize);
        }
        step = !step;
    }

    // addModel<Bridge>(glm::vec3(-5.0f, -0.40f, 0.0f), glm::vec3(1, 1, 1),
    //                  rp3d::BodyType::STATIC);
    // addModel<Hatchet>(glm::vec3(-2.0f, 0.0f, 0.0f),
    //                   glm::vec3(0.02f, 0.02f, 0.02f));
    // addModel<Commodore>(glm::vec3(-3.0f, 0.0f, 0.0f));
    // addModel<Box>(glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // addModel<House>(glm::vec3(-5.0f, -1.0f, -15.0f),
    //                 glm::vec3(3.0f, 2.0f, 3.0f), rp3d::BodyType::STATIC);
    // addModel<Skull>(glm::vec3(8.2f, 0.85f, -2.5f));
    // addModel<Box>(glm::vec3(-1.0f, 0.0f, -4.0f),
    // glm::vec3(1.0f, 1.0f, 1.0f));

    // Box bigBox =
    //     addModel<Box>(glm::vec3(0, 0, -5), glm::vec3(2.4f, 2.4f, 0.05f));
    // bigBox.physicsBody->setMass(10.0f);

    // create 64 models and add them to the vector
    // for (int x = 0; x < 3; x++) {
    //     for (int y = 0; y < 3; y++) {
    //         for (int z = 0; z < 3; z++) {
    //             Box box = addModel<Box>(glm::vec3(x - 1, y + 2, z - 5),
    //                                     glm::vec3(0.4f, 0.04f, 0.4f));
    //         }
    //     }
    // }

    std::cout << "Number of models: " << objects.size() << std::endl;
}

template <typename T>
T Render::addModel(glm::vec3 position, glm::vec3 scale,
                   rp3d::BodyType bodyType) {
    // std::cout << "Creating model of type: " << typeid(T).name() << std::endl;
    T* model =
        new T(scale, true, position, true, bodyType, world, &physicsCommon);

    // Get the name of the model class
    std::string modelClassName = typeid(*model).name();

    // Check if the model class has been loaded before
    if (loadedModelClasses.find(modelClassName) == loadedModelClasses.end()) {
        model->loadModelPath(&modelVertices, &modelIndices);
        model->setTextureId(
            vulkanSetup.createTexture(model->getTexturePath(), &commandPool));

        // Add the model class to the set of loaded model classes
        loadedModelClasses.insert(modelClassName);
    }

    objects.push_back(std::shared_ptr<T>(model));

    return *model;
}

void Render::createPhysicsWorld() {
    float earthGravity = 9.80665f;
    // Create the world settings
    rp3d::PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = 20;
    settings.isSleepingEnabled = false;
    settings.gravity = rp3d::Vector3(0, -earthGravity, 0);

    // Create the physics world with your settings
    world = physicsCommon.createPhysicsWorld(settings);
}

void Render::initVulkan() {
    std::cout << "Render::initVulkan()" << std::endl;
    vulkanSetup.createInstance();
    vulkanSetup.setupDebugMessenger();
    vulkanSetup.createSurface();
    vulkanSetup.pickPhysicalDevice();
    vulkanSetup.createLogicalDevice();
    vulkanSetup.createSwapChain();
    vulkanSetup.createImageViews();
    createRenderPass();
    shader.loadShaders();
    shader.createDescriptorSetLayout();
    shader.createGraphicsPipeline();
    createCommandPool();
    vulkanSetup.createDepthResources(&commandPool);
    vulkanSetup.createFramebuffers();

    createPhysicsWorld();

    createScene();

    vulkanSetup.createVertexBuffer(&commandPool, modelVertices);
    vulkanSetup.createIndexBuffer(&commandPool, modelIndices,
                                  sizeof(modelIndices[0]) *
                                      modelIndices.size());
    vulkanSetup.createUniformBuffers();
    vulkanSetup.createDescriptorPool();
    vulkanSetup.createDescriptorSets(&shader.descriptorSetLayout);

    // endof scene creation ~here or 6 lines above?

    createCommandBuffers();
    createSyncObjects();
}

void Render::initImgui() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.FontGlobalScale = 2.5f;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window.window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanSetup.instance;
    init_info.PhysicalDevice = vulkanSetup.physicalDevice;
    init_info.Device = vulkanSetup.device;
    init_info.Queue = vulkanSetup.graphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = vulkanSetup.descriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 2;

    ImGui_ImplVulkan_Init(&init_info, renderPass);
}

void Render::updateCharacterModelMatrix(glm::mat4 viewMatrix) {
    // std::cout << "Render::updateCharacterModelMatrix()" << std::endl;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.9f, -1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
}

void Render::drawFrame(FPSCamera::Matrices& matrices) {
    // https://vulkan-tutorial.com/en/Drawing_a_triangle/Drawing/Rendering_and_presentation#page_Outline-of-a-frame
    // At a high level, rendering a frame in Vulkan consists of a common set of
    // steps:

    //     Wait for the previous frame to finish
    vkWaitForFences(vulkanSetup.device, 1, &inFlightFences[currentFrame],
                    VK_TRUE, UINT64_MAX);

    //     Acquire an image from the swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        vulkanSetup.device, vulkanSetup.swapChain, UINT64_MAX,
        imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkanSetup.recreateSwapChain(&commandPool);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(currentFrame, matrices);

    // Only reset the fence if we are submitting work
    vkResetFences(vulkanSetup.device, 1, &inFlightFences[currentFrame]);

    //     Record a command buffer which draws the scene onto that image
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    //     Submit the recorded command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(vulkanSetup.graphicsQueue, 1, &submitInfo,
                      inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    //     Present the swap chain image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {vulkanSetup.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(vulkanSetup.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        window.framebufferResized) {
        window.framebufferResized = false;
        vulkanSetup.recreateSwapChain(&commandPool);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Render::updateUniformBuffer(uint32_t currentImage,
                                 FPSCamera::Matrices& matrices) {
    UniformBufferObject ubo{};

    ubo.model = glm::mat4(1.0f);
    ubo.view = matrices.view;
    ubo.proj = matrices.projection;

    // Using a UBO this way is not the most efficient way to pass frequently
    // changing values to the shader. A more efficient way to pass a small
    // buffer of data to shaders are push constants.
    // https://vulkan-tutorial.com/Uniform_buffers/Descriptor_layout_and_buffer
    memcpy(vulkanSetup.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Render::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(vulkanSetup.device, &semaphoreInfo, nullptr,
                              &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanSetup.device, &semaphoreInfo, nullptr,
                              &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanSetup.device, &fenceInfo, nullptr,
                          &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error(
                "failed to create synchronization objects for a frame!");
        }
    }
}

void Render::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanSetup.device, &allocInfo,
                                 commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Render::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                 uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.4f, 0.5f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = vulkanSetup.swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vulkanSetup.swapChainExtent;

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      shader.graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(vulkanSetup.swapChainExtent.width);
    viewport.height = static_cast<float>(vulkanSetup.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vulkanSetup.swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {vulkanSetup.vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, vulkanSetup.indexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);

    // [todo] have a `drawNode` func?
    // https://github.com/SaschaWillems/Vulkan/blob/master/examples/gltfscenerendering/gltfscenerendering.cpp#L239

    uint32_t indexCount;
    for (std::shared_ptr<Model>& object : objects) {
        int descriptorSetOffset = 0;
        if (currentFrame == 1) {
            descriptorSetOffset = vulkanSetup.textures.size();
        }
        vkCmdBindDescriptorSets(
            commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
            shader.pipelineLayout, 0, 1,
            &vulkanSetup
                 .descriptorSets[descriptorSetOffset + object->getTextureId()],
            0, nullptr);

        const glm::mat4 modelMatrix = object->getModelMatrix();

        vkCmdPushConstants(commandBuffer, shader.pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4),
                           &modelMatrix);

        vkCmdDrawIndexed(commandBuffer, object->getIndicesCount(), 1,
                         object->getIndexOffset(), object->getVertexOffset(),
                         0);
    }

    if (state.paused) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.0f, 0.4f, 0.5f, 1.0f);

        static int counter = 0;

        // Set ImGui window to span the width of the Vulkan window with a slim margin
        float margin = 8.0f; // slim margin
        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(
            vulkanSetup.swapChainExtent.width - 2 * margin,
            0 // auto height
        ));
        ImGui::Begin("state");

    ImGui::SliderFloat("spawn velocity", &state.newObjectVelocity, 0.0f, 75.0f, "%.1f");
    ImGui::SliderFloat("spawn mass", &state.newObjectMass, 0.0f, 40.0f, "%.1f");
    ImGui::SliderFloat("spawn scale", &state.newObjectScale, 0.0f, 2.0f, "%.1f");
    ImGui::SliderFloat("camera speed", &state.movementSpeed, 0.1f, 20.0f, "%.2f");
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::Text("Boxes = %d", state.noBoxes);

        if (ImGui::Button("Reset FOV")) {
            window.camera.fov = 60.0f;
            window.camera.updateFOV(0);
        }
        ImGui::SameLine();
        if (ImGui::SliderFloat("Camera FOV (60.0)", &window.camera.fov, 10.0f, 145.0f, "%.1f")) {
            window.camera.updateFOV(0);
        }

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        // ImGui::ShowDemoWindow();

        // ImGui::End();

        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
    }


    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Render::createCommandPool() {
    // std::cout << "Render::createCommandPool()" << std::endl;
    QueueFamilyIndices queueFamilyIndices =
        vulkanSetup.findQueueFamilies(vulkanSetup.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(vulkanSetup.device, &poolInfo, nullptr,
                            &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void Render::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vulkanSetup.swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    // index of the attachment description array
    colorAttachmentRef.attachment = 0;
    // layout to transition to before subpass begins
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = vulkanSetup.findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // index of the attachment reference array
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                          depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(vulkanSetup.device, &renderPassInfo, nullptr,
                           &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Render::cleanup() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    shader.destroyGraphicsPipeline();
    shader.destroyPipelineLayout();

    vkDestroyRenderPass(vulkanSetup.device, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(vulkanSetup.device, imageAvailableSemaphores[i],
                           nullptr);
        vkDestroySemaphore(vulkanSetup.device, renderFinishedSemaphores[i],
                           nullptr);
        vkDestroyFence(vulkanSetup.device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(vulkanSetup.device, commandPool, nullptr);

    shader.cleanup();
    vulkanSetup.cleanup();
}
