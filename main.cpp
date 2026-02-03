#include <iostream>

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

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file


// Book: Page 82 - 9.

vec2 movementInput = vec2(0.0f);

constexpr int InitWidth = 800;
constexpr int InitHeight = 600;
constexpr int ErrLogSize = 512;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    movementInput = vec2(
        (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1.0f : 0.0f),
        (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1.0f : 0.0f)
    );

    if (movementInput.x != 0.0f || movementInput.y != 0.0f) {
        movementInput = normalize(movementInput);
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

GLFWwindow* setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(InitWidth, InitHeight, "Title", nullptr, nullptr);

    if (window == nullptr) {
        fmt::println("Failed to create GLFW window");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fmt::println("Failed to initialize GLAD");
        return nullptr;
    }

    glViewport(0, 0, InitWidth, InitHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(InitWidth, InitHeight, "Title", nullptr, nullptr);

    if (window == nullptr) {
        fmt::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fmt::println("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, InitWidth, InitHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LESS);*/

    Camera camera;
    ShaderProgram program(FILE_PATH(shader.vert), FILE_PATH(shader.frag));
    Texture* texture = Texture::Load(FILE_PATH(image.png));
    std::vector<Cube*> cubes;
    
    cubes.push_back(new Cube(&program));
    cubes.push_back(new Cube(&program));

    cubes[0]->mPosition = vec3(-0.5f, 0.0f, 0.0f);
    cubes[0]->mModulate = vec4(1.0f, 0.0f, 1.0f, 1.0f);

    cubes[1]->mPosition = vec3(0.5f, 0.0f, 0.0f);
    cubes[1]->mModulate = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    Model model = Model(FILE_PATH(ak47.glb));

    unsigned int matrices_index = glGetUniformBlockIndex(program.getId(), "Matrices");
    glUniformBlockBinding(program.getId(), matrices_index, 0);

    unsigned int UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
    camera.Position = vec3(0.0f, 0.0f, -1.0f);
    

    dvec2 oldMousePos(0.0f);
    double oldTime = glfwGetTime();
    glfwGetCursorPos(window, &oldMousePos.x, &oldMousePos.y);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        double time = glfwGetTime();
        double delta = time - oldTime;
        oldTime = time;
        dvec2 mousePos;
        glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
        
        double sensitivity = 2.0;
        dvec2 mouseMovement = (mousePos - oldMousePos) * sensitivity * delta;
        oldMousePos = mousePos;
        camera.Rotation.y += static_cast<float>(mouseMovement.x);
        camera.Rotation.x = std::clamp(camera.Rotation.x + static_cast<float>(mouseMovement.y), glm::radians(-89.0f), glm::radians(89.0f));
        
        vec4 movement(
            movementInput.x * static_cast<float>(delta), 
            0.0f, 
            movementInput.y * static_cast<float>(delta),
            1.0f
            );

        camera.Move(movement);

        if (delta > 0.0) {
            glfwSetWindowTitle(window, (std::string("FPS: ") + std::to_string(static_cast<int>(1.0 / delta))).c_str());
        }
        mat4 projection = camera.GetProjectionMatrix(static_cast<float>(InitWidth) / static_cast<float>(InitHeight));
        mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        
        model.Draw(program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

