#include <glm/gtc/random.hpp>
#include "Table.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Table::mesh;
std::unique_ptr<ppgso::Texture> Table::texture;
std::unique_ptr<ppgso::Shader> Table::shader;

Table::Table() {
    scale *= 0.02f;
    scale.z *= 1.5f;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Table.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Table.obj");
}

bool Table::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Table::render(Scene &scene) {
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
