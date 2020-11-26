#include <glm/gtc/random.hpp>
#include "Wall.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/copied_vert_glsl.h>
#include <shaders/copied_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;

Wall::Wall() {
    scale = {12,12,1};

    // Initialize static resources if needed
    //if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
   // if (!shader) shader = std::make_unique<ppgso::Shader>(copied_vert_glsl, copied_frag_glsl);
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


   /* shader->setUniform("lightPos",scene.lightSource->position);
    shader->setUniform("lightColor",scene.lightSource->color);
    shader->setUniform("viewPos",scene.camera->position);
    shader->setUniform("objectColor",{1.0f, 0.5f, 0.31f});

    shader->setUniform("model",modelMatrix);
    shader->setUniform("view",scene.camera->viewMatrix);
    shader->setUniform("projection",scene.camera->projectionMatrix);*/
    // Set up light
    //shader->setUniform("LightDirection", scene.lightDirection);
  /*  lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);

    // light properties
    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.setFloat("material.shininess", 64.0f);*/
/*
    shader->setUniform("light.ambient", {0.6f, 0.6f, 0.6f});
    shader->setUniform("light.diffuse", {0.5f, 0.5f, 0.5f});
    shader->setUniform("light.specular", {1.0f, 1.0f, 1.0f});

    shader->setUniform("light.position",scene.lightSource->position);

    shader->setUniform("material.shininess", 64.0f);
    shader->setUniform("material.diffuse", *texture);
    shader->setUniform("material.specular", *texture);

    shader->setUniform("viewPos",scene.camera->position);

    shader->setUniform("projection", scene.camera->projectionMatrix);
    shader->setUniform("view", scene.camera->viewMatrix);
    shader->setUniform("model", modelMatrix);*/


    shader->setUniform("lightPos",scene.lightSource->position);
    shader->setUniform("lightColor",scene.lightSource->color);
    shader->setUniform("viewPosition",scene.camera->position);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
