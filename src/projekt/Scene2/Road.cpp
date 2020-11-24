#include <glm/gtc/random.hpp>
#include "Road.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Road::mesh;
std::unique_ptr<ppgso::Texture> Road::texture;
std::unique_ptr<ppgso::Shader> Road::shader;

Road::Road() {
    // Set random scale speed and rotation
    position.y += 2.0f;
    rotation = {3.15f,-3.15f,0.0f};
    scale *= 6.0f;
    scale.y *= 1.8f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Road.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Road::update(Scene &scene, float dt) {
    textureOffset.y -= dt/3;
    generateModelMatrix();

    return true;
}

void Road::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // Pass UV mapping offset to the shader
    shader->setUniform("TextureOffset", textureOffset);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
