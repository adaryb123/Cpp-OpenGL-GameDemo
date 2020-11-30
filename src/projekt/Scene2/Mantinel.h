#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "src/projekt/Scene.h"
#include "src/projekt/Object.h"

class Mantinel final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Age of the object in seconds
    float age{0.0f};

    // Speed and rotational momentum
    glm::vec2 textureOffset {0.1,0};
public:
    bool addingOffset = false;
    Mantinel();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;

private:
};