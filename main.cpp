#include <iostream>
#include <cstdarg>
#include <thread>
#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "shader_program.h"
#include "texture.h"

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file


// Book: Page 82 - 9.

constexpr int InitWidth = 800;
constexpr int InitHeight = 600;
constexpr int ErrLogSize = 512;
float moveDir = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    moveDir = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ? 1.0f : 0.0f);
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

    float offset = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glm::mat4 perspective = glm::perspective(
            glm::radians(45.0f), 
            static_cast<float>(InitWidth) / static_cast<float>(InitHeight), 
            0.1f, 
            100.0f
        );
        
        offset += moveDir * 0.001f;

        glm::vec3 cameraPosition(std::cos(offset) * 2.0f, 0.0f, std::sin(offset) * 2.0f);
        glm::mat4 cameraTransform = glm::identity<glm::mat4>();
        cameraTransform = glm::translate(cameraTransform, cameraPosition + glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 worldTransform = glm::identity<glm::mat4>();
        program.use();
        program.setFloat("offset", offset);
        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));
        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "cameraTransform"), 1, GL_FALSE, glm::value_ptr(cameraTransform));
        glUniformMatrix4fv(glGetUniformLocation(program.getId(), "transform"), 1, GL_FALSE, glm::value_ptr(worldTransform));

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

/*s Test : JPH::BroadPhaseLayerInterface
{
public:
    Test : JPH::BroadPhaseLayerInterface();
    ~Test : JPH::BroadPhaseLayerInterface();

private:

};

Test : JPH::BroadPhaseLayerInterface::Test : JPH::BroadPhaseLayerInterface()
{
}

Test : JPH::BroadPhaseLayerInterface::~Test : JPH::BroadPhaseLayerInterface()
{
}*/


void physics_main() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();

    JPH::RegisterTypes();

    JPH::TempAllocatorImpl temp_allocator(10 * 1024 * 1024);
    JPH::JobSystemThreadPool job_system(4, 0, std::thread::hardware_concurrency() - 1);

    const uint32_t cMaxBodies = 1024;
    const uint32_t cNumBodyMutexes = 0;
    const uint32_t cMaxBodyPairs = 1024;
    const uint32_t cMaxContactConstraints = 1024;

    JPH::BroadPhaseLayer bpli;


    JPH::PhysicsSystem physics_system;

}