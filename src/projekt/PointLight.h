#pragma once

#include <mesh.h>
#include "Object.h"
#include "Scene.h"

class PointLight : public Object{

    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
public:
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};
