#include <glm/gtc/random.hpp>
#include "Sphere.h"

#include <shaders/myshader_vert_glsl.h>
#include <shaders/myshader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Sphere::mesh;
std::unique_ptr<ppgso::Texture> Sphere::texture;
std::unique_ptr<ppgso::Shader> Sphere::shader;

Sphere::Sphere() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(myshader_vert_glsl, myshader_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Metal.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Sphere::update(Scene &scene, float dt) {
    myUpdate1(dt);
    child->myUpdate2(dt,modelMatrix);
    child->child->myUpdate3(dt,child->modelMatrix);
    return true;
}

void Sphere::render(Scene &scene) {

    shader->use();
    auto lightSource1 = dynamic_cast<PointLight*>(scene.pointLights.front().get());

    shader->setUniform("light.position",lightSource1->position);
    shader->setUniform("light.color",lightSource1->color);
    shader->setUniform("light.ambient",  lightSource1->ambient);
    shader->setUniform("light.diffuse",  lightSource1->diffuse);
    shader->setUniform("light.specular", lightSource1->specular);
    shader->setUniform("light.constant", lightSource1->constant);
    shader->setUniform("light.linear", lightSource1->linear);
    shader->setUniform("light.quadratic", lightSource1->quadratic);

    auto lightSource2 = dynamic_cast<PointLight*>(scene.pointLights.back().get());

    shader->setUniform("light2.position",lightSource2->position);
    shader->setUniform("light2.color",lightSource2->color);
    shader->setUniform("light2.ambient",  lightSource2->ambient);
    shader->setUniform("light2.diffuse",  lightSource2->diffuse);
    shader->setUniform("light2.specular", lightSource2->specular);
    shader->setUniform("light2.constant", lightSource2->constant);
    shader->setUniform("light2.linear", lightSource2->linear);
    shader->setUniform("light2.quadratic", lightSource2->quadratic);

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

    if (sphereNum == 1) {
        child->render(scene);
        child->child->render(scene);
    }
}

void Sphere::myUpdate1(float dt) {
    auto t = (float) glfwGetTime();
    position = {6*sin(t),4,8};

    glm::mat4 ROTATION_MATRIX = translate(glm::mat4{1.0f}, {rotation.x, rotation.y, 0})
                                * rotate(glm::mat4{1.0}, rotation.z, {0, 0, 1})
                                * translate(glm::mat4{1.0f}, {-rotation.x, -rotation.y, 0});

    modelMatrix = //modelMatrixParent *
                  translate(glm::mat4{1.0f}, this->position)
                  * ROTATION_MATRIX
                  * glm::scale(glm::mat4{1.0}, this->scale);


}

void Sphere::myUpdate2(float dt,glm::mat4 modelMatrixParent) {
    auto t = (float) glfwGetTime();
    position = {0,4,0};
    rotation = {0,-4, t*2};

    glm::mat4 ROTATION_MATRIX = translate(glm::mat4{1.0f}, {rotation.x, rotation.y, 0})
                                * rotate(glm::mat4{1.0}, rotation.z, {0, 0, 1})
                                * translate(glm::mat4{1.0f}, {-rotation.x, -rotation.y, 0});

    modelMatrix = modelMatrixParent *
                  translate(glm::mat4{1.0f}, this->position)
                  * ROTATION_MATRIX
                  * glm::scale(glm::mat4{1.0}, this->scale);
}

void Sphere::myUpdate3(float dt, glm::mat4 modelMatrixParent) {
    auto t = (float) glfwGetTime();
    position = {0,2,0};
    rotation = {0, -2, t};
    scale = {0.5,0.5,0.5};

    glm::mat4 ROTATION_MATRIX = translate(glm::mat4{1.0f}, {rotation.x, rotation.y, 0})
                                * rotate(glm::mat4{1.0}, rotation.z, {0, 0, 1})
                                * translate(glm::mat4{1.0f}, {-rotation.x, -rotation.y, 0});

    modelMatrix = modelMatrixParent *
                  translate(glm::mat4{1.0f}, this->position)
                  * ROTATION_MATRIX
                  * glm::scale(glm::mat4{1.0}, this->scale);
}