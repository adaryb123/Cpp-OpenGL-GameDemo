#include <glm/gtc/random.hpp>
#include "Sky.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Sky::mesh;
std::unique_ptr<ppgso::Texture> Sky::texture;
std::unique_ptr<ppgso::Shader> Sky::shader;

Sky::Sky() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Sky.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Sky::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Sky::render(Scene &scene) {
    glDepthMask(GL_FALSE);

    shader->use();

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", glm::mat4{1.0f});
    shader->setUniform("ProjectionMatrix", glm::mat4{1.0f});
    shader->setUniform("Texture", *texture);
    mesh->render();

    glDepthMask(GL_TRUE);
}
