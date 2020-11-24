#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include "src/projekt/Camera.h"
#include <map>

class CameraAsPlayer final : public Camera {
public:
    GLfloat movementSpeed= 3.0f;
    GLfloat sensitivity = 0.3f;

    glm::vec3 worldUp = {0,1,0};
    glm::vec3 right = {0,0,0};

    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;

    GLfloat lastMouseX = 0.0;
    GLfloat lastMouseY = 0.0;
    GLfloat mouseOffsetX = 0.0;
    GLfloat mouseOffsetY = 0.0;
    GLfloat firstMouse = true;

    CameraAsPlayer(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);
    void update(float time);

    void processMouseMovement(float time);
    void processKeyboardMovement(float time);
    void updateCameraVectors();
};

