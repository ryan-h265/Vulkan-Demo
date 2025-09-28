#include "Vertex.hpp"

VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3>
Vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    // This functions decribes the memory layout and shader attribute locations
    // See shaders/shaders.vert for the shader code
    // Look how the shader inputs correspond to the vertex attribute
    // descriptions

    attributeDescriptions[0].binding =
        0; // index of the binding in the array of bindings
    attributeDescriptions[0].location =
        0; // location of the attribute in the vertex shader
    // shader type : format
    // float: VK_FORMAT_R32_SFLOAT
    // vec2: VK_FORMAT_R32G32_SFLOAT
    // vec3: VK_FORMAT_R32G32B32_SFLOAT
    // vec4: VK_FORMAT_R32G32B32A32_SFLOAT
    // ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed
    // integers uvec4: VK_FORMAT_R32G32B32A32_UINT, a 4-component vector of
    // 32-bit unsigned integers double: VK_FORMAT_R64_SFLOAT, a double-precision
    // (64-bit) float
    attributeDescriptions[0].format =
        VK_FORMAT_R32G32B32_SFLOAT; // format of the data
    attributeDescriptions[0].offset = offsetof(
        Vertex, pos); // offset of the attribute in the vertex structure

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}
