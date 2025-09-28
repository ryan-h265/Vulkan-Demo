#include "House.hpp"
#include "Model.hpp"

int House::totalNbHouses = 0;
int House::textureId = 0;
int House::vertexOffset = 0;
int House::indexOffset = 0;
int House::indicesCount = 0;

House::House(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
             bool createRigidBody, rp3d::BodyType bodyType,
             rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbHouses, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/house.obj";
    TEXTURE_PATH = "textures/house.png";

    totalNbHouses++;
}
