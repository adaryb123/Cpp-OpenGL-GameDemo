#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>

// Forward declare a scene
class Scene;

class Object {
public:
    // Define default constructors as this is an abstract class
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;
    virtual ~Object() {};
    virtual bool update(Scene &scene, float dt) = 0;
    virtual void render(Scene &scene) = 0;
    //virtual void onClick(Scene &scene) {};

    // Object properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::mat4 modelMatrix{1};

    struct material{
        glm::vec3 ambient{1,1,1};
        glm::vec3 diffuse{1,1,1};
        glm::vec3 specular{1,1,1};
        float shininess = 0.25;
    } material;

  /*  struct material{
      glm::vec3 ambient{0.0969755f,0.093313708f,0.059977125f};
      glm::vec3 diffuse{0.443168417f,0.433226705f,0.307670788f};
      glm::vec3 specular{0.507886491f,0.520246116f,0.433642574f};
      float shininess = 0.23644578f;
    } material;*/

protected:
    void generateModelMatrix();
};
