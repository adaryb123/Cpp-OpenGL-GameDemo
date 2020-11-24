#include <glm/gtc/random.hpp>
#include "Mantinel.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Mantinel::mesh;
std::unique_ptr<ppgso::Texture> Mantinel::texture;
std::unique_ptr<ppgso::Shader> Mantinel::shader;

Mantinel::Mantinel() {
    scale = {6.0,2.0,1};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Roadblock.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Roadblock.obj");
}

bool Mantinel::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Mantinel::render(Scene &scene) {
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

