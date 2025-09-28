#include "Commodore.hpp"
#include "Model.hpp"

int Commodore::totalNbCommodores = 0;
int Commodore::textureId = 0;
int Commodore::vertexOffset = 0;
int Commodore::indexOffset = 0;
int Commodore::indicesCount = 0;

Commodore::Commodore(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
                     bool createRigidBody, rp3d::BodyType bodyType,
                     rp3d::PhysicsWorld* world,
                     rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbCommodores, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/low_spec_commodore_pet.obj";
    TEXTURE_PATH = "textures/c_pet_tex.png";

    totalNbCommodores++;
}
