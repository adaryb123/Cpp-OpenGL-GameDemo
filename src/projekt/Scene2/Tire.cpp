#include <glm/gtc/random.hpp>
#include "Tire.h"
#include "Mantinel.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tire::mesh;
std::unique_ptr<ppgso::Texture> Tire::texture;
std::unique_ptr<ppgso::Shader> Tire::shader;

Tire::Tire() {
    // Set random scale speed and rotation
    rotation = {0.0f,3.12,0.0f};
    rotMomentum = { 0,0,-6};
    speed = {8.0f,-10.0f,0};
    scale *= 0.10f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tire7.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Tire.obj");

    ySizeConst = 0.2f;
    xSizeConst = 0.7f;
}

bool Tire::update(Scene &scene, float dt) {
    // Count time alive
    age += dt;
    // Rotate the object
    rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Collide with scene
    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        //tires will bounce off mantinels
        auto mantinel = dynamic_cast<Mantinel *>(obj.get());
        if (mantinel) {
            if (abs(position.x) >= (abs(mantinel->position.x) - 1)) {
                speed.x = -speed.x;
                rotMomentum.z = -rotMomentum.z;
                if (position.x < 0)
                    position.x += 0.2f;
                else
                    position.x -= 0.2f;
                generateModelMatrix();
                return true;
            }
        }
    }
    position += speed * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    updateBoundingBox();
    return true;
}

void Tire::render(Scene &scene) {
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

void Tire::collide(std::string collisionType) {
    collided = true;
    if (collisionType.compare("FRONT") == 0){
        speed.x = 0;
        rotMomentum = {0,0,0};
        position.z -= 0.75f;
        speed.z -= 3.0f;
        rotMomentum.y = -5.0f;
    }
    else if (collisionType.compare("LEFT") == 0){
        speed.x = -speed.x;
        rotMomentum.z = -rotMomentum.z;
        position.x -= 0.3f;
      /*  position.x -= 0.5f;
        speed.x -= 2.0f;
        rotMomentum.z = 3.0f;*/
    }
    else if (collisionType.compare("RIGHT") == 0){
        speed.x = -speed.x;
        rotMomentum.z = -rotMomentum.z;
        position.x += 0.3f;
       /* position.x += 0.5f;
        speed.x += 2.0f;
        rotMomentum.z = -3.0f;*/
    }
}