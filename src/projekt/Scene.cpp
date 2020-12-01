#include "Scene.h"
#include "src/projekt/Scene1/FirstPersonCamera.h"

void Scene::update(float dt) {
    //update wind
   // srand (static_cast <unsigned> (10*time(0)));
   // wind.x =  -3.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.0f-(-3.0f))));
   wind.x = glm::linearRand(0.0f,4.5f);
   int swapWindChance = glm::linearRand(0,100);
   if (swapWindChance > 99)
      windDirection *= -1;
   wind *= windDirection;

    camera->keyboard = keyboard;
    camera->mouseX = mouseX;
    camera->mouseY = mouseY;
    camera->update(dt);

    for ( auto& obj : pointLights )
        obj->update(*this,dt);
  //  lightSource->update(*this,time);
    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, dt))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }
}

void Scene::render() {
    // Simply render all objects
    for ( auto& obj : pointLights )
        obj->render(*this);

    //lightSource->render(*this);
    for ( auto& obj : objects )
        obj->render(*this);
}

