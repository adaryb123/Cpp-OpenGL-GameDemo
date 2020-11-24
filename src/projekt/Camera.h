#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>

class Camera {
public:
    // Define default constructors as this is an abstract class
    Camera() = default;
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    virtual ~Camera() {};
    virtual void update(float dt) = 0;

    // Camera properties
    std::map< int, int > keyboard;
    GLfloat mouseX;
    GLfloat mouseY;
    glm::vec3 position = {0,0,0};
    glm::vec3 front = {0,0,1};
    glm::vec3 up = {0,1,0};
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

};