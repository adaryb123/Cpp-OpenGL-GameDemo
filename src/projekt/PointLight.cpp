#include <glm/gtc/random.hpp>
#include "PointLight.h"

#include <shaders/light_cube_vert_glsl.h>
#include <shaders/light_cube_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> PointLight::mesh;
std::unique_ptr<ppgso::Shader> PointLight::shader;

PointLight::PointLight() {
    scale = {0.2,0.2,0.2};
    position = {0.0,4.0,0.0};

    color = {1.0,1.0,1.0};
    ambient = {0.2f, 0.2f, 0.2f};
    diffuse  = {0.5f, 0.5f, 0.5f};
    specular = {1.0f, 1.0f, 1.0f};

    if (!shader) shader = std::make_unique<ppgso::Shader>(light_cube_vert_glsl, light_cube_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
}

bool PointLight::update(Scene &scene, float time) {
    generateModelMatrix();
    return true;
}

void PointLight::render(Scene &scene) {
    shader->use();
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}