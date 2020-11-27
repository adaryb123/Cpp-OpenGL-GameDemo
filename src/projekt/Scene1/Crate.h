#pragma once
#include <ppgso/ppgso.h>

#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

class Crate final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    Crate();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

