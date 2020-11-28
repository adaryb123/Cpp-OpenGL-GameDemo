#include <glm/gtc/random.hpp>
#include "Wall.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;

Wall::Wall() {
    scale = {12,12,1};

    //Pearl
    material.ambient = {	0.25f, 0.20725f, 0.20725f};
    material.diffuse = {1.0f, 0.829f, 0.829f};
    material.specular = {0.296648f, 0.296648f, 0.296648f};
    material.shininess = 0.088f;


    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Wall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Wall::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Wall::render(Scene &scene) {
    shader->use();

    //Light
    auto lightSource1 = dynamic_cast<PointLight*>(scene.pointLights.front().get());

    shader->setUniform("light.position",lightSource1->position);
    shader->setUniform("light.color",lightSource1->color);
    shader->setUniform("light.ambient",  lightSource1->ambient);
    shader->setUniform("light.diffuse",  lightSource1->diffuse);
    shader->setUniform("light.specular", lightSource1->specular);
    shader->setUniform("light.constant", lightSource1->constant);
    shader->setUniform("light.linear", lightSource1->linear);
    shader->setUniform("light.quadratic", lightSource1->quadratic);

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
