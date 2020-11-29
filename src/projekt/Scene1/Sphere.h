#pragma once
#include <ppgso/ppgso.h>

#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

class Sphere final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    Sphere();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void myUpdate1(float dt);

    void myUpdate2(float dt, glm::mat4 modelMatrixParent);

    void myUpdate3(float dt, glm::mat4 modelMatrixParent);

    int sphereNum;
    std::unique_ptr<Sphere> child;
};