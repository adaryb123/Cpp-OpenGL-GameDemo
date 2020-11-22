#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "Scene.h"
#include "Object.h"

class Magnet final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Age of the object in seconds
    float age{0.0f};

    // Speed and rotational momentum
    glm::vec3 speed;
    glm::vec3 rotMomentum;

public:
    Magnet();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
private:
};

