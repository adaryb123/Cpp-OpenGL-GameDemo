#include <glm/glm.hpp>

#include "CameraAsPlayer.h"


CameraAsPlayer::CameraAsPlayer(float fow, float ratio, float near, float far) {
    updateCameraAsPlayerVectors();
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void CameraAsPlayer::update(float dt) {
    if (CameraAsPlayerIsPlayer)
    {
        processKeyboardMovement(dt);
        processMouseMovement(dt);
        updateCameraAsPlayerVectors();
    }
    viewMatrix = lookAt(position, position + front, up);
}

glm::vec3 CameraAsPlayer::cast(double u, double v) {
    // Create point in Screen coordinates
    glm::vec4 screenPosition{u,v,0.0f,1.0f};

    // Use inverse matrices to get the point in world coordinates
    auto invProjection = glm::inverse(projectionMatrix);
    auto invView = glm::inverse(viewMatrix);

    // Compute position on the CameraAsPlayer plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;

    // Create direction vector
    auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
    return glm::vec3{direction};
}

void CameraAsPlayer::processKeyboardMovement(float dt)
{
    if(keyboard[GLFW_KEY_LEFT]) {
        this->position -= this->right * this->movementSpeed * dt;
        //position.x += 10 * dt;
    }
    if(keyboard[GLFW_KEY_RIGHT]) {
        this->position += this->right * this->movementSpeed * dt;
        //position.x -= 10 * dt;
    }
    if(keyboard[GLFW_KEY_UP]) {
        this->position += this->front * this->movementSpeed * dt;
        //position.z += 10 * dt;
    }
    if(keyboard[GLFW_KEY_DOWN]) {
        this->position -= this->front * this->movementSpeed * dt;
        //position.z -= 10 * dt;
    }
}

void CameraAsPlayer::processMouseMovement(float dt)             //pozret ci sa spravne pripocitava ten yoffset a xoffset a skusit to podla fejkovejkamery
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

    //this->pitch += static_cast<GLfloat>(mouseOffsetY) * this->sensitivity * dt;
    //this->yaw += static_cast<GLfloat>(mouseOffsetX) * this->sensitivity * dt;
    this->pitch += static_cast<GLfloat>(mouseOffsetY) * this->sensitivity ;//* dt;
    this->yaw += static_cast<GLfloat>(mouseOffsetX) * this->sensitivity ;//* dt;

    if (this->pitch > 80.0f)
        this->pitch = 80.0f;
    else if (this->pitch < -80.0f)
        this->pitch = -80.0f;

    if (this->yaw > 360.0f || this->yaw < -360.0f)
        this->yaw = 0.0f;
}

void CameraAsPlayer::updateCameraAsPlayerVectors()
{
    this->front.x = static_cast<float>(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));
    this->front.y = static_cast<float>(sin(glm::radians(this->pitch)));
    this->front.z = static_cast<float>(sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}