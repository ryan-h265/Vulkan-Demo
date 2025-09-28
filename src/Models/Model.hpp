#pragma once

#include <cmath>
#include <iostream>      // std::cout
#include <optional>      // std::optional
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include <glm/glm.hpp> // glm::mat4

#include <reactphysics3d/reactphysics3d.h>

#include "../Vertex.hpp"

class Model {
    int modelId;
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  protected:
    glm::mat4 model = glm::mat4(1.0f);
    std::string overrideTexturePath; // Allow per-instance texture override

  protected:


  public:
    Model(int modelId, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
          bool matrixOffset = false,
          glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
          bool createRigidBody = true,
          rp3d::BodyType bodyType = rp3d::BodyType::DYNAMIC,
          rp3d::PhysicsWorld* world = nullptr,
          rp3d::PhysicsCommon* physicsCommon = nullptr);

    std::string MODEL_PATH;
    std::string TEXTURE_PATH;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    rp3d::RigidBody* physicsBody = nullptr;

    void loadModelPath(std::vector<Vertex>* modelVertices,
                       std::vector<uint32_t>* modelIndices);

    void updateVelocity(glm::vec3 velocity);
    void updateAngularVelocity(glm::vec3 angularVelocity);

    int getModelId() { return modelId; }

    void setScale(glm::vec3 scale) {
        this->scale = scale;
        setModelMatrix(glm::scale(model, scale));
    }

    glm::mat4 getModelMatrix() { return model; }
    // setModelMatrix(glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f,
    // 0.0f, 1.0f)));
    void setModelMatrix(glm::mat4 model) {
        this->model = model;
        this->position = glm::vec3(model[3][0], model[3][1], model[3][2]);
    }

  // Returns the texture path, using override if set
  virtual std::string getTexturePath() const {
    if (!overrideTexturePath.empty()) return overrideTexturePath;
    return TEXTURE_PATH;
  }

  // Set a custom texture path for this model instance
  void setTexturePath(const std::string& path) { overrideTexturePath = path; }

    virtual int getModelCount() { return 0; }
    virtual int getTextureId() { return -1; }
    virtual void setTextureId(int id) {}

    virtual int getVertexOffset() { return -1; }
    virtual void setVertexOffset(int offset) {}

    virtual int getIndexOffset() { return -1; }
    virtual void setIndexOffset(int offset) {}

    virtual int getIndicesCount() { return -1; }
    virtual void setIndicesCount(int count) {}
};
