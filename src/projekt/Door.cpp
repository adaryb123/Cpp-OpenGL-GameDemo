#include <glm/gtc/random.hpp>
#include "Door.h"
#include "Camera.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Door::mesh;
std::unique_ptr<ppgso::Texture> Door::texture;
std::unique_ptr<ppgso::Shader> Door::shader;

Door::Door() {
    scale *= 0.02f;
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Door.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Door.obj");
}

bool Door::update(Scene &scene, float dt) {

    auto player = dynamic_cast<Camera*>(scene.camera.get());
    if (distance(player->position,position) < 3.5)
    {
        if(scene.keyboard[GLFW_KEY_F] && abs(player->front.x) < 0.75){
            scene.endScene = true;
        }
    }

    generateModelMatrix();
    return true;
}

void Door::render(Scene &scene) {
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
