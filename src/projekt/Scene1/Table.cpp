#include <glm/gtc/random.hpp>
#include "Table.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Table::mesh;
std::unique_ptr<ppgso::Texture> Table::texture;
std::unique_ptr<ppgso::Shader> Table::shader;

Table::Table() {
    scale *= 0.02f;
    scale.z *= 1.5f;
    rotation = {-1.5,0,0};
    //Gold
    material.ambient = {0.1745f,0.01175f,0.01175f};
    material.diffuse = {0.61424f,0.04136f,0.04136f};
    material.specular = {0.727811f,0.626959f,0.626959f};
    material.shininess = {0.6f};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Table.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Table.obj");
}

bool Table::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Table::render(Scene &scene) {

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
