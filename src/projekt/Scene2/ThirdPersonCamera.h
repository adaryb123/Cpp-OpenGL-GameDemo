#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include "src/projekt/Camera.h"
#include <map>

class ThirdPersonCamera final : public Camera {
public:
    ThirdPersonCamera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);
    void update(float time);

    bool followPlayer = false;

    void update(glm::vec3 position);
};

