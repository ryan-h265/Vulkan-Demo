#include "Bridge.hpp"
#include "Model.hpp"

int Bridge::totalNbBridges = 0;
int Bridge::textureId = 0;
int Bridge::vertexOffset = 0;
int Bridge::indexOffset = 0;
int Bridge::indicesCount = 0;

Bridge::Bridge(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
               bool createRigidBody, rp3d::BodyType bodyType,
               rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbBridges, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/bridge.obj";
    TEXTURE_PATH = "textures/bridge_diffuse.png";

    totalNbBridges++;
}
