#include "ObjectSpawnShuffler.h"
#include <glm/gtc/random.hpp>

void ObjectSpawnShuffler::shuffle(Scene& scene){
    srand (static_cast <unsigned> (time(0)));

    while (true) {
        for (auto &obj : objects) {
            obj->position.x =  -7.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.0f-(-7.0f))));
            obj->position.z =  -7.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.0f-(-7.0f))));
        }
        bool good = true;
        for (auto &obj1 : objects) {
            for (auto &obj2 : objects) {
                if (obj1.get() != obj2.get()) {
                    if (distance(obj1->position, obj2->position) < 2) {
                        good = false;
                        break;
                    }
                }
            }
        }

        if (good == true)
            break;
    }

    for (auto &obj : objects) {
        scene.objects.push_back(move(obj));
    }
}