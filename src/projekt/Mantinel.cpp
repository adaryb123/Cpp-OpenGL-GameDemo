#include <glm/gtc/random.hpp>
#include "Mantinel.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Mantinel::mesh;
std::unique_ptr<ppgso::Texture> Mantinel::texture;
std::unique_ptr<ppgso::Shader> Mantinel::shader;

Mantinel::Mantinel() {
    scale = {4.0,1,1};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Roadblock.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Roadblock.obj");
}

bool Mantinel::update(Scene &scene, float dt) {
    /*// Count time alive
    age += dt;

    // Animate position according to time
    position += speed * dt;

    // Rotate the object
    rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibility
    if (age > 10.0f || position.y < -10) return false;

    // Collide with scene
    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        // We only need to collide with asteroids and projectiles, ignore other objects
        auto asteroid = dynamic_cast<Asteroid*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
        auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
        if (!asteroid && !projectile) continue;

        // When colliding with other asteroids make sure the object is older than .5s
        // This prevents excessive collisions when asteroids explode.
        if (asteroid && age < 0.5f) continue;

        // Compare distance to approximate size of the asteroid estimated from scale.
        if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
            int pieces = 3;

            // Too small to split into pieces
            if (scale.y < 0.5) pieces = 0;

            // The projectile will be destroyed
            if (projectile) projectile->destroy();

            // Generate smaller asteroids
            explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

            // Destroy self
            return false;
        }
    }*/

    // rotation = {0.0f,1.5f,4.7};

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Mantinel::render(Scene &scene) {
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
