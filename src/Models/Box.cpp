#include <reactphysics3d/reactphysics3d.h>

#include "Box.hpp"
#include "Model.hpp"

int Box::totalNbBoxes = 0;
int Box::textureId = 0;
int Box::vertexOffset = 0;
int Box::indexOffset = 0;
int Box::indicesCount = 0;

Box::Box(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
         bool createRigidBody, rp3d::BodyType bodyType,
         rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbBoxes, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/cube.obj";
    TEXTURE_PATH = "textures/cube_rough.png";

    totalNbBoxes++;
}
