#pragma once
#include <ppgso/ppgso.h>

#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

class LightSwitch final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    //can be used every 0.5 second
    double cooldown;
public:
    LightSwitch();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

