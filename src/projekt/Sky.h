#ifndef PPGSO_SKY_H
#define PPGSO_SKY_H

#include <ppgso/ppgso.h>

#include "Object.h"
#include "Scene.h"

/*!
 * This object renders the scene background
 * It does not use the camera so it uses different 2D shader program
 * Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
 */
class Sky final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    Sky();
    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

#endif //PPGSO_SKY_H
