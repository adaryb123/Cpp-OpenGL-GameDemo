#include <glm/glm.hpp>

#include "FirstPersonCamera.h"


FirstPersonCamera::FirstPersonCamera(float fow, float ratio, float near, float far) {
    updateCameraVectors();
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void FirstPersonCamera::update(float dt) {
    processKeyboardMovement(dt);
    processMouseMovement(dt);
    updateCameraVectors();

    position.y = 1;
    if (position.x > 9)
        position.x = 9;
    else if (position.x < -9)
        position.x = -9;
    else if (position.z > 9)
        position.z = 9;
    else if (position.z < -9)
        position.z = -9;

    viewMatrix = lookAt(position, position + front, up);
}

void FirstPersonCamera::processKeyboardMovement(float dt)
{
    if(keyboard[GLFW_KEY_LEFT]) {
        this->position -= this->right * this->movementSpeed * dt;
    }
    if(keyboard[GLFW_KEY_RIGHT]) {
        this->position += this->right * this->movementSpeed * dt;
    }
    if(keyboard[GLFW_KEY_UP]) {
        this->position += this->front * this->movementSpeed * 1.5f * dt;
    }
    if(keyboard[GLFW_KEY_DOWN]) {
        this->position -= this->front * this->movementSpeed * 1.5f * dt;
    }
}

void FirstPersonCamera::processMouseMovement(float dt)
{
    if (this->firstMouse == true)
    {
        this->lastMouseX = this->mouseX;
        this->lastMouseY = this->mouseY;
        this->firstMouse = false;
    }

    //Calc offset
    this->mouseOffsetX = this->mouseX - this->lastMouseX;
    this->mouseOffsetY = this->lastMouseY - this->mouseY;

    //Set last X and Y
    this->lastMouseX = this->mouseX;
    this->lastMouseY = this->mouseY;


    this->sensitivity = 2.5;
    this->pitch += static_cast<GLfloat>(mouseOffsetY) * this->sensitivity * dt;
    this->yaw += static_cast<GLfloat>(mouseOffsetX) * this->sensitivity * dt;

    if (this->pitch > 80.0f)
        this->pitch = 80.0f;
    else if (this->pitch < -80.0f)
        this->pitch = -80.0f;

    if (this->yaw > 360.0f || this->yaw < -360.0f)
        this->yaw = 0.0f;
}

void FirstPersonCamera::updateCameraVectors()
{
    this->front.x = static_cast<float>(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));
    this->front.y = static_cast<float>(sin(glm::radians(this->pitch)));
    this->front.z = static_cast<float>(sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}