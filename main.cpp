#include <iostream>

#include "engine.h"
#include "main_game_object.h"
#include <GLFW/glfw3.h>

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

int main()
{
    Engine::Setup();

    Engine engine;

    MainGameObject* main_game_object = new MainGameObject();
    engine.add_game_object(main_game_object);

    double lastFrameTime = 0.0;

    while (true)
    {
        double frameTime = glfwGetTime();
        double delta = frameTime - lastFrameTime;
        lastFrameTime = frameTime;

        if (delta > 0.0) {
            engine.Update(delta);
        }
    }

    Engine::Dismantle();

    glfwTerminate();
    return 0;
}


