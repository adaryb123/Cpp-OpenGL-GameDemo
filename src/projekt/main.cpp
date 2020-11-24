#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "Camera.h"
#include "scene.h"
#include "Player.h"
#include "Road.h"
#include "Generator.h"
#include "Mantinel.h"
#include "Sky.h"
#include "Tire.h"
#include "Wall.h"
#include "Door.h"

const unsigned int HEIGHT = 1200;
const unsigned int WIDTH = 1800;

class SceneWindow : public ppgso::Window {
private:
    Scene scene2;
    Scene scene1;
    // 1 for the first scene and 2 for the second scene
    int current_scene = 2;
    bool animate = true;
    void initScene2() {
        current_scene = 2;
        scene2.objects.clear();
        scene2.cameraIsPlayer = false;

        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {0.0,-10.0,-10.0};
        camera->front = {0.0,1.0,1.0};
        scene2.camera = move(camera);
        //scene2.objects.push_back(move(camera));

        scene2.objects.push_back(std::make_unique<Sky>());

        scene2.objects.push_back(std::make_unique<Road>());

        auto generator = std::make_unique<Generator>();
        generator->position.y = 12.5f;
        scene2.objects.push_back(move(generator));

        auto right_mantinel = std::make_unique<Mantinel>();
        right_mantinel->position.x = 6.5;
        right_mantinel->rotation =  {0.0,-1.58,1.65};
        scene2.objects.push_back(move(right_mantinel));

        auto left_mantinel = std::make_unique<Mantinel>();
        left_mantinel->position.x = -6.5;
        left_mantinel->rotation = {0.0,-1.58,1.45};
        scene2.objects.push_back(move(left_mantinel));

        auto player = std::make_unique<Player>();
        player->position.y = -6;
        scene2.objects.push_back(move(player));
    }
    void initScene1() {
        current_scene = 1;
        scene1.objects.clear();
        scene1.cameraIsPlayer = 1;

        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {0.0,0.0,-1.0};
        camera->front = {0.0,0.0,1.0};
        scene1.camera = move(camera);
        //scene1.objects.push_back(move(camera));

        auto tire = std::make_unique<Tire>();
        tire->position = {0,0,-5};
        tire->erase_speed();
        //left_mantinel->rotation = {0.0,-1.58,1.45};
        scene1.objects.push_back(move(tire));

        auto wall1 = std::make_unique<Wall>();      //front
        wall1->position = {0,1,-10};
        wall1->rotation = {0.0,0,0};
        scene1.objects.push_back(move(wall1));

        auto wall2 = std::make_unique<Wall>();      //down
        wall2->position = {0,-2,0};
        wall2->rotation = {-1.5,3,0};
        scene1.objects.push_back(move(wall2));

        auto wall3 = std::make_unique<Wall>();      //back
        wall3->position = {0,1,10};
        wall3->rotation = {3,0,0};
        scene1.objects.push_back(move(wall3));

        auto wall4 = std::make_unique<Wall>();      //up
        wall4->position = {0,10,0};
        wall4->rotation = {1.5,3,0};
        scene1.objects.push_back(move(wall4));

        auto wall5 = std::make_unique<Wall>();      //right
        wall5->position = {10,0,0};
        wall5->rotation = {0,0,-1.5};
        scene1.objects.push_back(move(wall5));

        auto wall6 = std::make_unique<Wall>();      //left
        wall6->position = {-10,0,0};
        wall6->rotation = {0,0,1.5};
        scene1.objects.push_back(move(wall6));

        auto door = std::make_unique<Door>();
        door->position = {-3,3,-9};
        door->rotation = {1.5,0,3};
        scene1.objects.push_back(move(door));
    }

public:
    SceneWindow() : Window{"GoKart Arena", WIDTH, HEIGHT} {
        //Scene2Window() : Window{"gl9_scene21", SIZE, SIZE} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        // Disable cursor
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        initScene2();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene1.keyboard[key] = action;
        scene2.keyboard[key] = action;

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene2();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            animate = !animate;
        }

        if (key == GLFW_KEY_N && action == GLFW_PRESS) {
            initScene1();
        }
    }

    void onCursorPos(double cursorX, double cursorY) override {
        scene1.mouseX = static_cast<float>(cursorX);
        scene1.mouseY = static_cast<float>(cursorY);
       // scene1.cursor.x = cursorX;
       // scene1.cursor.y = cursorY;
    }


    void onIdle() override {
        // Track time
        if (current_scene == 2) {
            static auto time = (float) glfwGetTime();

            // Compute time delta
            if (scene2.stopAnimation == true)
                animate = false;

            if (scene2.stopAnimation == false && animate == false)
                animate = true;

            float dt = animate ? (float) glfwGetTime() - time : 0;

            time = (float) glfwGetTime();

            // Set gray background
            glClearColor(.5f, .5f, .5f, 0);
            // Clear depth and color buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update and render all objects
            scene2.update(dt);
            scene2.render();
        }
        else if (current_scene == 1)
        {
            static auto time = (float) glfwGetTime();

            float dt = animate ? (float) glfwGetTime() - time : 0;

            time = (float) glfwGetTime();

            // Set gray background
            glClearColor(.5f, .5f, .5f, 0);
            // Clear depth and color buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update and render all objects
            scene1.update(dt);
            scene1.render();
        }
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}

/*
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
*/