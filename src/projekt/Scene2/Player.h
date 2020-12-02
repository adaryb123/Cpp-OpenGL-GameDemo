#pragma once
#include <ppgso/ppgso.h>

#include "src/projekt/Object.h"

class Player final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    //if the wind is toggled or not
    bool countWithWind = false;
    // the wind can be toggled every 0.5 sec
    double cooldown;

    //If player jumps, he cant collide with obstacles
    bool airborne = false;
    //if he is jumping up
    bool goingUp = false;
    //if he is falling down
    bool falling = false;
public:
    Player();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

