#include <glm/gtc/random.hpp>
#include "Barrel.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Barrel::mesh;
std::unique_ptr<ppgso::Texture> Barrel::texture;
std::unique_ptr<ppgso::Shader> Barrel::shader;

Barrel::Barrel() {
    // Initialize static resources if needed
    scale = {0.1,0.1,0.15};
    rotation = {1.6,0,1.5};
    //Silver
    material.ambient = {0.19225f, 0.19225f, 0.19225f};
    material.diffuse = {0.50754f, 0.50754f, 0.50754f};
    material.specular = {0.508273f, 0.508273f, 0.508273f};
    material.shininess = 0.4f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("OilBarrel.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("OilBarrel.obj");
}

bool Barrel::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Barrel::render(Scene &scene) {

    shader->use();
    auto lightSource1 = dynamic_cast<PointLight*>(scene.pointLights.front().get());

    shader->setUniform("light.position",lightSource1->position);
    shader->setUniform("light.color",lightSource1->color);
    shader->setUniform("light.ambient",  lightSource1->ambient);
    shader->setUniform("light.diffuse",  lightSource1->diffuse);
    shader->setUniform("light.specular", lightSource1->specular);
    shader->setUniform("light.constant", lightSource1->constant);
    shader->setUniform("light.linear", lightSource1->linear);
    shader->setUniform("light.quadratic", lightSource1->quadratic);

    auto lightSource2 = dynamic_cast<PointLight*>(scene.pointLights.back().get());

    shader->setUniform("light2.position",lightSource2->position);
    shader->setUniform("light2.color",lightSource2->color);
    shader->setUniform("light2.ambient",  lightSource2->ambient);
    shader->setUniform("light2.diffuse",  lightSource2->diffuse);
    shader->setUniform("light2.specular", lightSource2->specular);
    shader->setUniform("light2.constant", lightSource2->constant);
    shader->setUniform("light2.linear", lightSource2->linear);
    shader->setUniform("light2.quadratic", lightSource2->quadratic);

    //Material
    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

    //Camera
    shader->setUniform("viewPosition",scene.camera->position);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
