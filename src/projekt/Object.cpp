#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "Object.h"

void Object::generateModelMatrix() {
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
}

void Object::updateBoundingBox() {
    boundingBox.min_y = position.y + ySizeConst;
    boundingBox.max_y = position.y - ySizeConst;
    boundingBox.min_x = position.x + xSizeConst;
    boundingBox.max_x = position.x - xSizeConst;
}