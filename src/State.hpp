#pragma once

struct gameState {
    bool paused = false;
    bool wireframe = false;

    int noBoxes = 0;
    int noUserIntendedBoxes = 0;

    float newObjectVelocity = 25.0f;
    float newObjectScale = 0.2f;
    float newObjectMass = 0.3f;

    float movementSpeed = 2.5f;
};
