#include <glm/glm.hpp>

#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void ThirdPersonCamera::update(float dt) {

    viewMatrix = lookAt(position, position + front, up);
}

void ThirdPersonCamera::update(glm::vec3 newPosition) {
    position.x = newPosition.x;
    viewMatrix = lookAt(position, position + front, up);
}
