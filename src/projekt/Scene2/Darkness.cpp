#include <glm/gtc/random.hpp>

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "Darkness.h"


// Static resources
std::unique_ptr<ppgso::Mesh> Darkness::mesh;
std::unique_ptr<ppgso::Texture> Darkness::texture;
std::unique_ptr<ppgso::Shader> Darkness::shader;

Darkness::Darkness() {
    // Set random scale speed and rotation
    position = {-0.5,35,-1};
    scale = {15,15,30};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Darkness.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Darkness::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Darkness::render(Scene &scene) {
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

