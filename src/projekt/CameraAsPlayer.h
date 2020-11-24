#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include <map>
#include "Camera.h"

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class CameraAsPlayer final : public Camera {
public:

    GLfloat movementSpeed= 3.0f;
    GLfloat sensitivity = 0.3f;

    glm::vec3 worldUp = {0,1,0};
    glm::vec3 position = {0,0,0};
    glm::vec3 front = {0,0,1};
    glm::vec3 right = {0,0,0};
    glm::vec3 up = {0,1,0};

    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;
    //GLfloat roll= 0.0f;

    GLfloat lastMouseX = 0.0;
    GLfloat lastMouseY = 0.0;
    GLfloat mouseX = 0.0;
    GLfloat mouseY = 0.0;
    GLfloat mouseOffsetX = 0.0;
    GLfloat mouseOffsetY = 0.0;
    GLfloat firstMouse = true;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    CameraAsPlayer(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

    void update(float time);

    //glm::vec3 cast(double u, double v);


    // Keyboard state
    std::map< int, int > keyboard;
    bool cameraIsPlayer;
    void processMouseMovement(float time);
    void processKeyboardMovement(float time);
    void updateCameraVectors();
};

