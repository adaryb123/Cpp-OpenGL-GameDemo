#include <glm/gtc/random.hpp>
#include "LightSwitch.h"
#include "FirstPersonCamera.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>
/*
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>*/

// Static resources
std::unique_ptr<ppgso::Mesh> LightSwitch::mesh;
std::unique_ptr<ppgso::Texture> LightSwitch::texture;
std::unique_ptr<ppgso::Shader> LightSwitch::shader;

LightSwitch::LightSwitch() {
    // Initialize static resources if needed
    //scale = {0.005,0.005,0.005};
    material.ambient = {0.0969755f,0.093313708f,0.059977125f};                  //stale to je velmi svetle
    material.diffuse = {0.443168417f,0.433226705f,0.307670788f};
    material.specular = {0.507886491f,0.520246116f,0.433642574f};
    material.shininess = 0.23644578f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("lightswitch.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lightswitch.obj");
}

bool LightSwitch::update(Scene &scene, float dt) {

    auto player = dynamic_cast<FirstPersonCamera*>(scene.camera.get());
    if (distance(player->position,position) < 3.5)
    {
        if(scene.keyboard[GLFW_KEY_F] && abs(player->front.x) < 0.75){
            auto lightSource1 = dynamic_cast<PointLight*>(scene.pointLights.front().get());
            lightSource1->changeColor();
            auto lightSource2 = dynamic_cast<PointLight*>(scene.pointLights.back().get());
            lightSource2->changeColor();
        }
    }

    generateModelMatrix();
    return true;
}

void LightSwitch::render(Scene &scene) {

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
