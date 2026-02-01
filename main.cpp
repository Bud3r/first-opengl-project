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

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file


// Book: Page 82 - 9.

constexpr int InitWidth = 800;
constexpr int InitHeight = 600;
constexpr int ErrLogSize = 512;
float moveDir = 0.0f;

void setup_uniform_buffer();

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    moveDir = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ? 1.0f : 0.0f);
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
    /*{
        SetupJolt();
        PhysicsServer physics_server;
        EndJolt();
    }*/

    /*GLFWwindow* window = setupWindow();

    if (!window)
        return -1;*/

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

    ShaderProgram program(FILE_PATH(shader.vert), FILE_PATH(shader.frag));
    Texture texture(FILE_PATH(image.png));
    std::vector<Cube*> cubes;
    
    cubes.push_back(new Cube(&program));
    cubes.push_back(new Cube(&program));

    cubes[0]->mPosition = vec3(-0.5f, 0.0f, 0.0f);
    cubes[0]->mModulate = vec4(1.0f, 0.0f, 1.0f, 1.0f);

    cubes[1]->mPosition = vec3(0.5f, 0.0f, 0.0f);
    cubes[1]->mModulate = vec4(1.0f, 0.0f, 0.0f, 1.0f);


    unsigned int matrices_index = glGetUniformBlockIndex(program.getId(), "Matrices");
    glUniformBlockBinding(program.getId(), matrices_index, 0);

    unsigned int UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
    
    glm::mat4 perspective = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(InitWidth) / static_cast<float>(InitHeight),
        0.1f,
        100.0f
    );

    mat4 view = glm::identity<mat4>();
    view = translate(view, vec3(0.0f, 0.0f, -1.0f));

    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(perspective));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        
        //glUniformMatrix4fv(program.getLocation("view2"), 1, GL_FALSE, glm::value_ptr(view));
        

        for (auto &cube : cubes)
        {
            cube->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

