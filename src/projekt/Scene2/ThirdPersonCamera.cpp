#include <glm/glm.hpp>

#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void ThirdPersonCamera::update(float dt) {

    if(keyboard[GLFW_KEY_E])
        cameraMode++;

    if (cameraMode > 3)
        cameraMode = 0;

    if (cameraMode == 0)
    {
        position = {0.0,-10.0,-10.0};
        front = {0.0,1.0,1.0};
    }
    else if (cameraMode == 1) {
        position = {0.0, -12.0, -5.0};
        front = {0.0, 2.0, 1.0};
        position.x = playerPosition.x;
    }
    else if (cameraMode == 2) {
        position = {0.0,-2.5,-15.0};
        front = {0.0,0.15,1.0};
    }
    else if (cameraMode == 3) {
        float t = (float) glfwGetTime();
        position = {-2*sin(t)+playerPosition.x, 3*cos(t)-4.5,-12.0};
        front = {0.0,0.15,1.0};
    }

    viewMatrix = lookAt(position, position + front, up);
}