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

        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {0.0,-10.0,-10.0};
        camera->back = {0.0,-1.0,-1.0};
        scene2.camera = move(camera);

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

        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {0.0,-10.0,-10.0};
        camera->back = {0.0,-1.0,-1.0};
        scene1.camera = move(camera);

        auto left_mantinel = std::make_unique<Mantinel>();
        left_mantinel->position.x = -6.5;
        left_mantinel->rotation = {0.0,-1.58,1.45};
        scene1.objects.push_back(move(left_mantinel));

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
