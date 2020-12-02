#include <glm/gtc/random.hpp>

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "TrafficCone.h"


// Static resources
std::unique_ptr<ppgso::Mesh> TrafficCone::mesh;
std::unique_ptr<ppgso::Texture> TrafficCone::texture;
std::unique_ptr<ppgso::Shader> TrafficCone::shader;

TrafficCone::TrafficCone() {
    rotation = {-3.0f,0.0f,0.0f};
    rotMomentum = {0,0,0};
    speed = {0,-10.0f,0};
    scale *= 0.03f;

    ySizeConst = 0.2f;
    xSizeConst = 0.2f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("TrafficCone.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("TrafficCone.obj");
}

bool TrafficCone::update(Scene &scene, float dt) {
    // Delete when alive longer than 10s or out of visibility
    age += dt;
    if (age > 10.0f || position.y < -10) return false;

    rotation += rotMomentum * dt;
    position += speed * dt;

    generateModelMatrix();
    updateBoundingBox();
    return true;
}

void TrafficCone::render(Scene &scene) {
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

void TrafficCone::collide(const std::string& collisionType) {
    collided = true;
    if (collisionType == "FRONT"){
        position.z -= 0.75f;
        speed.z -= 3.0f;
        rotMomentum.x = -5.0f;
    }
    else if (collisionType == "LEFT"){
        position.x -= 0.5f;
        speed.x -= 2.0f;
        rotMomentum.z = 3.0f;
    }
    else if (collisionType == "RIGHT"){
        position.x += 0.5f;
        speed.x += 2.0f;
        rotMomentum.z = -3.0f;
    }
}