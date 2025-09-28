#include "Skull.hpp"
#include "Model.hpp"

int Skull::totalNbSkulls = 0;
int Skull::textureId = 0;
int Skull::vertexOffset = 0;
int Skull::indexOffset = 0;
int Skull::indicesCount = 0;

Skull::Skull(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
             bool createRigidBody, rp3d::BodyType bodyType,
             rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbSkulls, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/skull_monster.obj";
    TEXTURE_PATH = "textures/Skull_Monster_512.png";

    totalNbSkulls++;
}
