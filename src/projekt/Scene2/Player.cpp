#include "Player.h"
#include "src/projekt/Scene.h"
#include "Mantinel.h"
#include "Tire.h"
#include "TrafficCone.h"
#include "src/projekt/Scene2/Magnet.h"
#include "windows.h"
#include "ThirdPersonCamera.h"
#include "Finish.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
    // Scale the default model
    scale = {0.5f,0.5f,1.0f};
    rotation.z += 3.2f;
    under_force = false;

    // Initialize static resources if need3ed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("RacingCar.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("RacingCar.obj");

    ySizeConst = 2.5f;
    xSizeConst = 0.6f;
}

bool Player::update(Scene &scene, float dt) {
    // Hit detection
    for ( auto& obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        auto finish = dynamic_cast<Finish*>(obj.get());
        if (finish)
        {
            if (position.y > finish->position.y) {
                scene.stopAnimation = true;
               // scene.endScene = true;
                return true;
            }
        }

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
        // If player hits an obstacle, game stops for a moment
        auto tire = dynamic_cast<Tire*>(obj.get());
        if(tire && airborne == false) {
            if (tire->boundingBox.max_y <= boundingBox.min_y && tire->boundingBox.max_y >= boundingBox.max_y)
                if ((tire->boundingBox.min_x >= boundingBox.max_x && tire->boundingBox.min_x <= boundingBox.min_x ) || (tire->boundingBox.max_x >= boundingBox.max_x && tire->boundingBox.max_x <= boundingBox.min_x))
                {
                    if (tire->boundingBox.max_y >=  boundingBox.min_y -0.5)
                        tire->collide("FRONT");
                    else {
                        if (tire->position.x < position.x)
                            tire->collide("LEFT");
                        else
                            tire->collide("RIGHT");
                    }
                }
            }

        // If player hits an obstacle, game stops for a moment
        auto trafficCone = dynamic_cast<TrafficCone*>(obj.get());
        if(trafficCone && airborne == false && trafficCone->collided == false) {

            if (trafficCone->boundingBox.max_y <= boundingBox.min_y && trafficCone->boundingBox.max_y >= boundingBox.max_y)
                if ((trafficCone->boundingBox.min_x >= boundingBox.max_x && trafficCone->boundingBox.min_x <= boundingBox.min_x ) || (trafficCone->boundingBox.max_x >= boundingBox.max_x && trafficCone->boundingBox.max_x <= boundingBox.min_x))
                    {
                        if (trafficCone->boundingBox.max_y >=  boundingBox.min_y -1.0)
                            trafficCone->collide("FRONT");
                        else {
                            if (trafficCone->position.x < position.x)
                                trafficCone->collide("LEFT");
                            else
                                trafficCone->collide("RIGHT");
                        }
                    }
        }
        // Magnet will push the player a bit
        /*auto magnet = dynamic_cast<Magnet*>(obj.get());
        if(magnet) {
            if (magnet->position.y - position.y < 2 && magnet->position.y - position.y > -2) {
                auto magnet_distance = distance(position, magnet->position);
                if (magnet_distance < 8) {
                    under_force = true;
                    external_force.x = 12 - magnet_distance;
                    if (magnet->position.x < 0)
                        external_force = - external_force;
                }
            }
        }*/
    }
    // Move left and right
    if(scene.keyboard[GLFW_KEY_LEFT])
        position.x += 10 * dt;
    else if(scene.keyboard[GLFW_KEY_RIGHT])
        position.x -= 10 * dt;

    //Toggle wind
    if(scene.keyboard[GLFW_KEY_W])
    {
        countWithWind = !countWithWind;
    }
    if (countWithWind == true)
        position += scene.wind * dt;

    //Jump
    if((scene.keyboard[GLFW_KEY_SPACE] || goingUp == true) && falling == false) {
        if (goingUp == true)
            position.z -= 10 * dt;
        else
            goingUp = true;
        airborne = true;
    }
    if (position.z < -3) {
        position.z = -3;
        goingUp = false;
        falling = true;
    }

    if (airborne == true && goingUp == false)
        position += scene.gravity * dt;

    if (position.z > 0) {
        position.z = 0;
        airborne = false;
        falling = false;
    }
    /*if (under_force == true)
    {
        position -= external_force *dt;
        under_force = false;
    }*/

    auto camera = dynamic_cast<ThirdPersonCamera*>(scene.camera.get());
    camera->playerPosition = position;
    generateModelMatrix();
    updateBoundingBox();
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
