#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "Generator.h"
#include "src/projekt/Scene2/Tire.h"
#include "src/projekt/Scene2/TrafficCone.h"
#include "src/projekt/Scene2/Magnet.h"
#include "Finish.h"

Generator::Generator() {
    starting_time = glfwGetTime();
}

bool Generator::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    if (glfwGetTime() - starting_time > 60 && finish == false){
        auto obj = std::make_unique<Finish>();
        obj->position.y = position.y;
        scene.objects.push_back(move(obj));
        finish = true;
    }



    // Add object to scene when time reaches certain level
    if (time > 0.5) {

        //what object will generate - 0 for Tire, 1 for TrafficCone, 2 for Magnet
        int object_type = static_cast<int>(glm::linearRand(0, 2));          //no magnets for time being

        if (object_type == 0) {
            auto obj = std::make_unique<Tire>();
            obj->position = position;
            obj->position.x += glm::linearRand(-6.0f, 6.0f);
            obj->position.z -= 1.0f;
            scene.objects.push_back(move(obj));
            time = 0;
        }
        else if (object_type == 1) {
            auto obj = std::make_unique<TrafficCone>();
            obj->position = position;
            obj->position.x += glm::linearRand(-6.0f, 6.0f);
            scene.objects.push_back(move(obj));
            time = 0;
        }

        /*else if (object_type == 2) {
            auto obj = std::make_unique<Magnet>();
            obj->position = position;
            obj->position.x += 7.0f;
            obj->position.z -= 2.8f;

            int left = glm::linearRand(0, 1);
            if (left == 0)
            {
                obj->position.x *= -1;
                obj->rotation.y *= -1;
                obj->rotation.z -= 0.8f;
            }
            scene.objects.push_back(move(obj));
            time = 0;
        }*/
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
