#pragma once
#include <ppgso/ppgso.h>

#include "src/projekt/Object.h"

class Player final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    bool under_force = false;
    glm::vec3 external_force = {0,0,0};
    bool countWithWind = true;
public:
    Player();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

