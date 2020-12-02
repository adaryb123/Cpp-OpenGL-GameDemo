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

// Static resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
    scale = {0.5f,0.5f,1.0f};
    rotation.z += 3.2f;

    ySizeConst = 2.5f;
    xSizeConst = 0.6f;

    cooldown = glfwGetTime();

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

        //if player hits finnish, game ends
        auto finish = dynamic_cast<Finish*>(obj.get());
        if (finish)
        {
            if (position.y > finish->position.y) {
                scene.endScene = true;
                return true;
            }
        }

        // If player hits mantinel, he can move no further in that direction
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

        // If player hits an obstacle, call the collide metod of the obstacle
        auto tire = dynamic_cast<Tire*>(obj.get());
        if(tire && airborne == false) {
            if (tire->boundingBox.max_y <= boundingBox.min_y && tire->boundingBox.max_y >= boundingBox.max_y)
                if ((tire->boundingBox.min_x >= boundingBox.max_x && tire->boundingBox.min_x <= boundingBox.min_x ) || (tire->boundingBox.max_x >= boundingBox.max_x && tire->boundingBox.max_x <= boundingBox.min_x))
                {
                    if (tire->boundingBox.max_y >=  boundingBox.min_y -0.5)
                        tire->collide("FRONT");         //collision from front
                    else {
                        if (tire->position.x < position.x)
                            tire->collide("LEFT");      //collision from left
                        else
                            tire->collide("RIGHT");     //collision from right
                    }
                }
            }

        // If player hits an obstacle, call the collide metod of the obstacle
        auto trafficCone = dynamic_cast<TrafficCone*>(obj.get());
        if(trafficCone && airborne == false && trafficCone->collided == false) {

            if (trafficCone->boundingBox.max_y <= boundingBox.min_y && trafficCone->boundingBox.max_y >= boundingBox.max_y)
                if ((trafficCone->boundingBox.min_x >= boundingBox.max_x && trafficCone->boundingBox.min_x <= boundingBox.min_x ) || (trafficCone->boundingBox.max_x >= boundingBox.max_x && trafficCone->boundingBox.max_x <= boundingBox.min_x))
                    {
                        if (trafficCone->boundingBox.max_y >=  boundingBox.min_y -1.0)
                            trafficCone->collide("FRONT");          //collision from front
                        else {
                            if (trafficCone->position.x < position.x)
                                trafficCone->collide("LEFT");      //collision from left
                            else
                                trafficCone->collide("RIGHT");     //collision from right
                        }
                    }
        }
    }
    // Move left and right
    if(scene.keyboard[GLFW_KEY_LEFT])
        position.x += 10 * dt;
    else if(scene.keyboard[GLFW_KEY_RIGHT])
        position.x -= 10 * dt;

    //Toggle wind
    if(scene.keyboard[GLFW_KEY_W]) {
        if (glfwGetTime() - cooldown > 0.5) {
            countWithWind = !countWithWind;
            cooldown = glfwGetTime();
        }
    }
    // If wind is toggled, adjust position
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
    if (position.z < -3) {      //this is the "peak" of the jump
        position.z = -3;
        goingUp = false;
        falling = true;
    }

    if (airborne == true && goingUp == false)       //when player reaches the "peak of the jump", gravity will start working on him
        position += scene.gravity * dt;

    if (position.z > 0) {       //when player reaches the ground, gravity will stop working again
        position.z = 0;
        airborne = false;
        falling = false;
    }

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
