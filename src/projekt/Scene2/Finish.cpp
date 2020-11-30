#include <glm/gtc/random.hpp>
#include "Finish.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Finish::mesh;
std::unique_ptr<ppgso::Texture> Finish::texture;
std::unique_ptr<ppgso::Shader> Finish::shader;

Finish::Finish() {
    // Set random scale speed and rotation
    position = {0,0,-0.2};
    rotation = {3.15f,-3.15f,0.0f};
    scale.x = 6;
    speed = {0,-6.0f,0};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Finish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Finish::update(Scene &scene, float dt) {
    position += speed * dt;

    generateModelMatrix();

    return true;
}

void Finish::render(Scene &scene) {
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
