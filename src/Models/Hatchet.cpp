#include "Hatchet.hpp"
#include "Model.hpp"

int Hatchet::totalNbHatchets = 0;
int Hatchet::textureId = 0;
int Hatchet::vertexOffset = 0;
int Hatchet::indexOffset = 0;
int Hatchet::indicesCount = 0;

Hatchet::Hatchet(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
                 bool createRigidBody, rp3d::BodyType bodyType,
                 rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbHatchets, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/hatchet.obj";
    TEXTURE_PATH = "textures/hatchet_diffuse.png";

    totalNbHatchets++;
}
