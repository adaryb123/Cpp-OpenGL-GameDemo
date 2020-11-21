#include "Player.h"
#include "Scene.h"
#include "Mantinel.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

auto default_rotation = 3.2f;
glm::vec3 default_scale = {0.5f,0.5f,1.0f};

Player::Player() {
    // Scale the default model
    scale = default_scale;
    rotation.z += default_rotation;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("RacingCar.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("RacingCar.obj");
}

bool Player::update(Scene &scene, float dt) {
    // Hit detection
    for ( auto& obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // If player hits mantinel, he can move no further
        auto mantinel = dynamic_cast<Mantinel*>(obj.get());
        if (mantinel)
        {
            if (abs(position.x) >= (abs(mantinel->position.x)-1)) {
                if (position.x < 0)
                    position.x += 0.01f;
                else
                    position.x -= 0.01f;
                return true;
            }
        }
    }
    // Keyboard controls
    if(scene.keyboard[GLFW_KEY_LEFT])
        position.x += 10 * dt;
    else if(scene.keyboard[GLFW_KEY_RIGHT])
        position.x -= 10 * dt;

    generateModelMatrix();
    return true;
}

void Player::render(Scene &scene) {
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

void Player::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
