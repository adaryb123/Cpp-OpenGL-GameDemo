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
