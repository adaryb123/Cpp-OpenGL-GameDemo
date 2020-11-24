#include <glm/gtc/random.hpp>
#include "Magnet.h"
#include "Mantinel.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Magnet::mesh;
std::unique_ptr<ppgso::Texture> Magnet::texture;
std::unique_ptr<ppgso::Shader> Magnet::shader;

Magnet::Magnet() {
    // Set random scale speed and rotation
    rotation = {0.0f,1.5f,0.0f};
    rotMomentum = { 0,0,0};
    speed = {0.0f,-6.0f,0};
    scale *= 0.30f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Magnet.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Magnet.obj");
}

bool Magnet::update(Scene &scene, float dt) {
    // Count time alive
    age += dt;

    // Animate position according to time
    //position += speed * dt;

    // Rotate the object
    //rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Collide with scene
    /*for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        //Magnets will bounce off mantinels
        auto mantinel = dynamic_cast<Mantinel *>(obj.get());
        if (mantinel) {
            if (abs(position.x) >= (abs(mantinel->position.x) - 1)) {
                speed.x = -speed.x;
                if (position.x < 0)
                    position.x += 0.2f;
                else
                    position.x -= 0.2f;
                generateModelMatrix();
                return true;
            }
        }
    }*/
    position += speed * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Magnet::render(Scene &scene) {
    glClear(GL_DEPTH_BUFFER_BIT);

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