#pragma once
#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

class Generator final : public Object {
public:
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
    float time = 0.0f;
};