#include "Scene.h"
#include "src/projekt/Scene1/FirstPersonCamera.h"

void Scene::update(float time) {
    camera->keyboard = keyboard;
    camera->mouseX = mouseX;
    camera->mouseY = mouseY;
    camera->update(time);

    for ( auto& obj : pointLights )
        obj->update(*this,time);
  //  lightSource->update(*this,time);
    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, time))
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