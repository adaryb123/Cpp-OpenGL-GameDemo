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
    rotation = {0.0f,3.12,0.0f};
    rotMomentum = { 0,0,-6};
    speed = {8.0f,-10.0f,0};
    scale *= 0.10f;

    ySizeConst = 0.2f;
    xSizeConst = 0.7f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tire7.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Tire.obj");
}

bool Tire::update(Scene &scene, float dt) {
    age += dt;
    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Rotate the object
    rotation += rotMomentum * dt;

    // Bounce off a mantinel
    for (auto &obj : scene.objects) {

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

//If this objects collides with player, it will animate in specific way
void Tire::collide(const std::string& collisionType) {
    collided = true;
    if (collisionType == "FRONT"){
        speed.x = 0;
        rotMomentum = {0,0,0};
        position.z -= 0.75f;
        speed.z -= 1.0f;
        rotMomentum.y = -5.0f;
    }
    else if (collisionType == "LEFT"){
        speed.x = -speed.x;
        rotMomentum.z = -rotMomentum.z;
        position.x -= 0.3f;
    }
    else if (collisionType == "RIGHT"){
        speed.x = -speed.x;
        rotMomentum.z = -rotMomentum.z;
        position.x += 0.3f;
    }
}