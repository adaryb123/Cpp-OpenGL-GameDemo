#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>

/* Abstract camera class */
class Camera {
public:
    Camera() = default;
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    virtual ~Camera() {};
    virtual void update(float dt) = 0;

    //store keyboard and mouse input from screen
    std::map< int, int > keyboard;
    float mouseX;
    float mouseY;

    //propertiesň
    glm::vec3 position = {0,0,0};
    glm::vec3 front = {0,0,1};
    glm::vec3 up = {0,1,0};
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

};