#pragma once
#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

//This object spawn obstacles (Traffic cones and Tires) as well as Finish line
class Generator final : public Object {
public:
    Generator();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
    //time from last spawn
    float time = 0.0f;
    //finish spawns only once, after 60 seconds
    bool finish = false;
    double starting_time;
};
