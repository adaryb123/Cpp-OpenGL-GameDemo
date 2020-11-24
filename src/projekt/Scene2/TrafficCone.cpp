#include <glm/gtc/random.hpp>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "TrafficCone.h"


// Static resources
std::unique_ptr<ppgso::Mesh> TrafficCone::mesh;
std::unique_ptr<ppgso::Texture> TrafficCone::texture;
std::unique_ptr<ppgso::Shader> TrafficCone::shader;

TrafficCone::TrafficCone() {
    // Set random scale speed and rotation
    rotation = {-3.0f,0.0f,0.0f};

    speed = {0,-6.0f,0};
    scale *= 0.03f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("TrafficCone.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("TrafficCone.obj");
}

bool TrafficCone::update(Scene &scene, float dt) {
    // Count time alive
    age += dt;

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    position += speed * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void TrafficCone::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

