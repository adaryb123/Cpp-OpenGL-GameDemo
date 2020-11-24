#include <glm/glm.hpp>

#include "StaticCamera.h"


StaticCamera::StaticCamera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void StaticCamera::update(float dt) {

    viewMatrix = lookAt(position, position + front, up);
}
