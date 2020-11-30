#pragma once

#include <memory>
#include <map>
#include <list>

#include "Object.h"
#include "Camera.h"
#include "src/projekt/Scene1/PointLight.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
public:

    void update(float time);
    void render();

    // ThirdPersonCamera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection;//{-1.0f, -1.0f, -1.0f};

    //std::unique_ptr<Object> lightSource;
    std::list< std::unique_ptr<Object> > pointLights;

    //Wind
    glm::vec3 wind = {0,0,0};
    int windDirection = 1;

    // Store cursor state
    float mouseX;
    float mouseY;

    bool stopAnimation = false;
    bool endScene = false;
};

