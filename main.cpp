#include <iostream>
#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_program.h"
#include "texture.h"

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

// Book: Page 63 - 7.7

constexpr int InitWidth = 800;
constexpr int InitHeight = 600;
constexpr int ErrLogSize = 512;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    FILE_PATH(shader.vert);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    ShaderProgram program(FILE_PATH(shader.vert), FILE_PATH(shader.frag));
    Texture texture(FILE_PATH(image.png));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.use();
        float offset = static_cast<float>(clock()) / static_cast<float>(CLOCKS_PER_SEC) * 0.1f;
        program.setFloat("offset", offset);
        //program.setInt("outTexture", 0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteBuffers(1, &VBO);
    //glDeleteVertexArrays(1, &VAO);
    
    glfwTerminate();
    return 0;
}