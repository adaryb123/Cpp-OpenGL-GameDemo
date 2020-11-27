#include <glm/gtc/random.hpp>
#include "Crate.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Crate::mesh;
std::unique_ptr<ppgso::Texture> Crate::texture;
std::unique_ptr<ppgso::Shader> Crate::shader;

Crate::Crate() {
    // Initialize static resources if needed

    //Brass
    material.ambient = {0.329412f, 0.223529f, 0.027451f};
    material.diffuse = {0.780392f, 0.568627f, 0.113725f};
    material.specular = {0.992157f, 0.941176f, 0.807843f};
    material.shininess = 0.21794872f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Wood.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
}

bool Crate::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Crate::render(Scene &scene) {

    shader->use();
    auto lightSource1 = dynamic_cast<PointLight*>(scene.lightSource.get());

    shader->setUniform("light.position",lightSource1->position);
    shader->setUniform("light.color",lightSource1->color);
    shader->setUniform("light.ambient",  lightSource1->ambient);
    shader->setUniform("light.diffuse",  lightSource1->diffuse);
    shader->setUniform("light.specular", lightSource1->specular);

    //Material
    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

    //Camera
    shader->setUniform("viewPosition",scene.camera->position);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}
