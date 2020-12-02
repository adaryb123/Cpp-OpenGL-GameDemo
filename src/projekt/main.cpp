#include <map>

#include <ppgso/ppgso.h>
#include <src/projekt/Scene2/Tire.h>
#include <src/projekt/Scene1/Sphere.h>
#include <src/projekt/Scene1/Barrel.h>
#include <src/projekt/Scene1/ObjectSpawnShuffler.h>
#include <src/projekt/Scene2/Darkness.h>

#include "src/projekt/Scene2/ThirdPersonCamera.h"
#include "src/projekt/Scene2/Player.h"
#include "src/projekt/Scene2/Road.h"
#include "src/projekt/Scene2/Generator.h"
#include "src/projekt/Scene2/Mantinel.h"
#include "src/projekt/Scene2/Sky.h"
#include "src/projekt/Scene1/Wall.h"
#include "src/projekt/Scene1/Door.h"
#include "src/projekt/Scene1/FirstPersonCamera.h"
#include "src/projekt/Scene1/Table.h"
#include "src/projekt/Scene1/Crate.h"
#include "src/projekt/Scene1/LightSwitch.h"

const unsigned int HEIGHT = 1200;
const unsigned int WIDTH = 1800;

class SceneWindow : public ppgso::Window {
private:
    Scene scene1;       //the entry room
    Scene scene2;       //the game itself

    // 1 for the first scene and 2 for the second scene
    int current_scene = 1;

    void initScene1() {
        current_scene = 1;
        scene1.objects.clear();
        scene1.endScene = false;

        //camera
        auto camera = std::make_unique<FirstPersonCamera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {0.0,0.0,-1.0};
        camera->front = {0.0,0.0,1.0};
        scene1.camera = move(camera);

        //6 walls to make a room
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
        wall3->rotation = {3.13,0,0};
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

        //Door to the next scene
        auto door = std::make_unique<Door>();
        door->position = {-3,3,-9.8};
        door->rotation = {1.55,0,3};
        scene1.objects.push_back(move(door));

        //2 lamps
        auto light_source1 = std::make_unique<PointLight>();
        light_source1->position = {9.7,8,0};
        scene1.pointLights.push_back(move(light_source1));

        auto light_source2 = std::make_unique<PointLight>();
        light_source2->position = {-9.7,8,0};
        scene1.pointLights.push_back(move(light_source2));

        //Lightswitch
        auto lightswitch= std::make_unique<LightSwitch>();
        lightswitch->position = {3,1.0,-9.8};
        lightswitch->rotation = {-1.55,3,0};
        scene1.objects.push_back(move(lightswitch));

        //3 moving spheres
        auto sphere1 = std::make_unique<Sphere>();
        sphere1->sphereNum = 1;
        auto sphere2 = std::make_unique<Sphere>();
        sphere2->sphereNum = 2;
        auto sphere3 = std::make_unique<Sphere>();
        sphere3->sphereNum = 2;
        sphere2->child = move(sphere3);
        sphere1->child = move(sphere2);
        scene1.objects.push_back(move(sphere1));

        //This object will randomize the spawn positions of the furniture
        auto shuffler = new ObjectSpawnShuffler();

        //The furniture
        auto table = std::make_unique<Table>();
        table->position = {5,-2,0};

        auto crate = std::make_unique<Crate>();
        crate->position = {-3,-1,2};

        auto barrel = std::make_unique<Barrel>();
        barrel->position = {-3,0.7,-1};

        shuffler->objects.push_back(move(table));
        shuffler->objects.push_back(move(crate));
        shuffler->objects.push_back(move(barrel));
        shuffler->shuffle(scene1);
        delete(shuffler);
    }

    void initScene2() {
        current_scene = 2;
        scene2.objects.clear();
        scene2.endScene = false;

        //Camera
        auto camera = std::make_unique<ThirdPersonCamera>(60.0f, 1.0f, 0.1f, 100.0f);
        scene2.camera = move(camera);

        //Road
        scene2.objects.push_back(std::make_unique<Road>());

        //Darkness object at the end of the tunnel
        scene2.objects.push_back(std::make_unique<Darkness>());

        //Object that spawns obstacles
        auto generator = std::make_unique<Generator>();
        generator->position.y = 25.0f;
        scene2.objects.push_back(move(generator));

        //right wall
        auto right_mantinel = std::make_unique<Mantinel>();
        right_mantinel->position.x = 6.5;
        right_mantinel->position.y = 17.0;
        right_mantinel->rotation =  {3.15,-1.56,1.60};
        right_mantinel->addingOffset = true;
        scene2.objects.push_back(move(right_mantinel));

        //left wall
        auto left_mantinel = std::make_unique<Mantinel>();
        left_mantinel->position.x = -6.0f;
        left_mantinel->position.y = 17.0;
        left_mantinel->rotation = {0,-1.56,1.55};
        scene2.objects.push_back(move(left_mantinel));

        //the player == red car
        auto player = std::make_unique<Player>();
        player->position.y = -6;
        scene2.objects.push_back(move(player));

    }

public:
    SceneWindow() : Window{"GoKart Arena", WIDTH, HEIGHT} {
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        current_scene = 1;
        initScene1();
    }
    void onKey(int key, int scanCode, int action, int mods) override {
        scene1.keyboard[key] = action;
        scene2.keyboard[key] = action;

        // Reset scene 2
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            if (current_scene == 2)
                initScene2();
        }

        // Return to scene 1
        if (key == GLFW_KEY_B && action == GLFW_PRESS) {
            if (current_scene == 2)
                initScene1();
        }
    }

    void onCursorPos(double cursorX, double cursorY) override {
        scene1.mouseX = static_cast<float>(cursorX);
        scene1.mouseY = static_cast<float>(cursorY);
    }
    /* We render scene 1 until the door is used , then we render scene2 */
    void onIdle() override {
        if (current_scene == 1) {
            // Track time
            static auto time = (float) glfwGetTime();
            float dt =  (float) glfwGetTime() - time;
            time = (float) glfwGetTime();

            // Set gray background
            glClearColor(.5f, .5f, .5f, 0);
            // Clear depth and color buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update and render all objects
            scene1.update(dt);
            scene1.render();

            if (scene1.endScene == true) {
                scene1.objects.clear();
                current_scene = 2;
                initScene2();
            }
        } else if (current_scene == 2) {
            if (scene2.endScene == true) {
                scene2.objects.clear();
                return;
            }
            // Track time
            static auto time = (float) glfwGetTime();
            float dt = !scene2.endScene ? (float) glfwGetTime() - time : 0;
            time = (float) glfwGetTime();

            // Set gray background
            glClearColor(.5f, .5f, .5f, 0);
            // Clear depth and color buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update and render all objects
            scene2.update(dt);
            scene2.render();
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
