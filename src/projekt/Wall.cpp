#include <glm/gtc/random.hpp>
#include "Wall.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;

Wall::Wall() {
    // Set random scale speed and rotation
    /*position.y += 2.0f;
    rotation = {3.15f,-3.15f,0.0f};
    scale *= 6.0f;
    scale.y *= 1.8f;*/
    //position = {0,1,-10};
    scale = {12,12,1};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Wall::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Wall::render(Scene &scene) {
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
