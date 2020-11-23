#include <glm/glm.hpp>

#include "Camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    updateCameraVectors();
    float fowInRad = (ppgso::PI/180.0f) * fow;
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update(float dt) {
    if (cameraIsPlayer)
    {
        processKeyboardMovement(dt);
        processMouseMovement(dt);
        updateCameraVectors();
    }
    viewMatrix = lookAt(position, position + front, up);
}

glm::vec3 Camera::cast(double u, double v) {
    // Create point in Screen coordinates
    glm::vec4 screenPosition{u,v,0.0f,1.0f};

    // Use inverse matrices to get the point in world coordinates
    auto invProjection = glm::inverse(projectionMatrix);
    auto invView = glm::inverse(viewMatrix);

    // Compute position on the camera plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;

    // Create direction vector
    auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
    return glm::vec3{direction};
}

void Camera::processKeyboardMovement(float dt)
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

void Camera::processMouseMovement(float dt)             //pozret ci sa spravne pripocitava ten yoffset a xoffset a skusit to podla fejkovejkamery
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

void Camera::updateCameraVectors()
{
    this->front.x = static_cast<float>(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));
    this->front.y = static_cast<float>(sin(glm::radians(this->pitch)));
    this->front.z = static_cast<float>(sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}