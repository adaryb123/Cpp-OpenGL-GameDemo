#include <glm/gtc/random.hpp>

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "TrafficCone.h"


// Static resources
std::unique_ptr<ppgso::Mesh> TrafficCone::mesh;
std::unique_ptr<ppgso::Texture> TrafficCone::texture;
std::unique_ptr<ppgso::Shader> TrafficCone::shader;

TrafficCone::TrafficCone() {
    // Set random scale speed and rotation
    rotation = {-3.0f,0.0f,0.0f};
    rotMomentum = {0,0,0};
    speed = {0,-10.0f,0};
    scale *= 0.03f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("TrafficCone.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("TrafficCone.obj");

    ySizeConst = 0.2f;
    xSizeConst = 0.2f;
}

bool TrafficCone::update(Scene &scene, float dt) {

    // Count time alive
    age += dt;

    rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    position += speed * dt;

    // Generate modelMatrix from position, rotation and scale
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

void TrafficCone::collide(std::string collisionType) {
    collided = true;
    if (collisionType.compare("FRONT") == 0){
        position.z -= 0.75f;
        speed.z -= 3.0f;
        rotMomentum.x = -5.0f;
    }
    else if (collisionType.compare("LEFT") == 0){
        position.x -= 0.5f;
        speed.x -= 2.0f;
        rotMomentum.z = 3.0f;
    }
    else if (collisionType.compare("RIGHT") == 0){
        position.x += 0.5f;
        speed.x += 2.0f;
        rotMomentum.z = -3.0f;
    }
}