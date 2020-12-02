#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>

class Scene;
/* Abstract Object class */
class Object {
public:
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;
    virtual ~Object() {};
    virtual bool update(Scene &scene, float dt) = 0;
    virtual void render(Scene &scene) = 0;

    // properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::mat4 modelMatrix{1};

    //material for lighting purposes
    struct material{
        glm::vec3 ambient{1,1,1};
        glm::vec3 diffuse{1,1,1};
        glm::vec3 specular{1,1,1};
        float shininess = 0.25;
    } material;

    //bounding box for collision purposes
    struct boundingBox{
        float min_x;
        float min_y;
        float max_x;
        float max_y;
    }boundingBox;

    //constants to help determine size of object
    float ySizeConst;
    float xSizeConst;

protected:
    void generateModelMatrix();
    void updateBoundingBox();
};
