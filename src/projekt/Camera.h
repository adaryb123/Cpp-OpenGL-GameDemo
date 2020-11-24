#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
//#include "Scene.h"
#include <map>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
    /*glm::vec3 up{0,1.0f,-1.0f};
    glm::vec3 position{0,-7.0f,-0.0f};
    glm::vec3 back{0,-6.0f,-1.0f};*/
    /*glm::vec3 up{0,1,0};
    glm::vec3 position{0,0,0};
    glm::vec3 back{0,0,-1};*/

    GLfloat movementSpeed= 3.0f;
    GLfloat sensitivity = 0.3f;

    glm::vec3 worldUp = {0,1,0};
    glm::vec3 position = {0,0,0};
    glm::vec3 front = {0,0,1};
    glm::vec3 right = {0,0,0};
    glm::vec3 up = {0,1,0};

    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;
    //GLfloat roll= 0.0f;

    GLfloat lastMouseX = 0.0;
    GLfloat lastMouseY = 0.0;
    GLfloat mouseX = 0.0;
    GLfloat mouseY = 0.0;
    GLfloat mouseOffsetX = 0.0;
    GLfloat mouseOffsetY = 0.0;
    GLfloat firstMouse = true;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    /*!
     * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
     * @param fow - Field of view in degrees
     * @param ratio - Viewport screen ratio (usually width/height of the render window)
     * @param near - Distance to the near frustum plane
     * @param far - Distance to the far frustum plane
     */
    Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

    /*!
     * Update Camera viewMatrix based on up, position and back vectors
     */
    void update(float time);

    /*!
     * Get direction vector in world coordinates through camera projection plane
     * @param u - camera projection plane horizontal coordinate [-1,1]
     * @param v - camera projection plane vertical coordinate [-1,1]
     * @return Normalized vector from camera position to position on the camera projection plane
     */
    glm::vec3 cast(double u, double v);


    // Keyboard state
    std::map< int, int > keyboard;

    // Store cursor state
    /*struct {
        double x, y;
        bool left, right;
    } cursor;*/

    bool cameraIsPlayer;
    void processMouseMovement(float time);
    void processKeyboardMovement(float time);
    void updateCameraVectors();
};

