#include <iostream>

#include "engine.h"
#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "shader_program.h"
#include "texture.h"
#include "physics_server.h"
#include "cube.h"
#include "camera.h"
#include "time.h"
#include "model.h"
#include "physics_body.h"
#include "game_process_object.h"

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

int main()
{
    Engine::Setup();
    Engine engine;

    GameProcessObject* gpo = new GameProcessObject();
    engine.AddProcessObject(gpo);

    
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


