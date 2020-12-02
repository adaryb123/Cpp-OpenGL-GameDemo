#pragma once

#include <memory>
#include <map>
#include <list>

#include "Object.h"
#include "Camera.h"
#include "Scene1/PointLight.h"

/* Scene is a class that stores all necessary data for one of the game scenes */
class Scene {
public:

    void update(float time);
    void render();

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

    // Directional light
    glm::vec3 lightDirection = {3.0, -2.0f, -2.0f};

    // List of point lights
    std::list< std::unique_ptr<Object> > pointLights;

    //Wind
    glm::vec3 wind = {0,0,0};
    int windDirection = 1;

    //Gravity
    glm::vec3 gravity = {0,0,5};

    // Keyboard and mouse states
    std::map< int, int > keyboard;
    float mouseX;
    float mouseY;

    //equals true if the scene should end
    bool endScene = false;
};

