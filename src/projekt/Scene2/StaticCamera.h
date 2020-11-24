#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include "src/projekt/Camera.h"
#include <map>

class StaticCamera final : public Camera {
public:
    StaticCamera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);
    void update(float time);
};

