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
    time += dt;

    //finish spawns only once, after 60 seconds
    if (glfwGetTime() - starting_time > 60 && finish == false){
        auto obj = std::make_unique<Finish>();
        obj->position.y = position.y;
        scene.objects.push_back(move(obj));
        finish = true;
    }

    //Spawn random obstacle every 0.5 seconds
    if (time > 0.5) {

        //what object will generate - 0 for Tire, 1 for TrafficCone
        int object_type = static_cast<int>(glm::linearRand(0, 2));

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
    }

    return true;
}

void Generator::render(Scene &scene) {
    // Generator will not be rendered
}
