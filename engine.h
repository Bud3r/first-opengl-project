#pragma once

#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "shader_program.h"
#include "texture.h"
#include "physics_server.h"
#include "cube.h"
#include "camera.h"
#include "time.h"
#include "model.h"
#include "physics_body.h"
#include "resource_loader.h"
#include "process_object.h"

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

constexpr int UBOBinding = 0;
constexpr int InitWidth = 800;
constexpr int InitHeight = 600;
constexpr int ErrLogSize = 512;

class Engine
{
public:
	Engine();
	~Engine();
	static void Setup();
	static void Dismantle();
	void Update(double deltaTime);
	void AddProcessObject(ProcessObject* process_object);
	PhysicsServer physics_server;
	ResourceLoader resource_loader;
	glm::vec2 movementInput;
	glm::dvec2 mouseMovement;
	glm::dvec2 mousePos;
	Camera* current_camera;
	std::vector<ProcessObject*> process_objects;
	const ShaderProgram& GetDefaultShaderProgram() const;
private:
	void ProcessInput(GLFWwindow* window);
	GLFWwindow* CreateWindow();
	GLFWwindow* m_window;
	ShaderProgram default_shader_program;

	unsigned int UBO;
	glm::dvec2 lastMousePos;
	double oldTime;
};


void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

const ShaderProgram& Engine::GetDefaultShaderProgram() const {
	return default_shader_program;
}

void Engine::AddProcessObject(ProcessObject* process_object) {
	process_object->engine = this;
	process_objects.push_back(process_object);
	process_object->AddedToEngine();
}

void Engine::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	movementInput = vec2(
		(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1.0f : 0.0f),
		(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1.0f : 0.0f) + (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1.0f : 0.0f)
	);

	if (movementInput.x != 0.0f || movementInput.y != 0.0f) {
		movementInput = glm::normalize(movementInput);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

Engine::Engine() {
	m_window = CreateWindow();

	glfwGetCursorPos(m_window, &lastMousePos.x, &lastMousePos.y);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, UBOBinding, UBO, 0, 2 * sizeof(glm::mat4));

	default_shader_program = ShaderProgram(FILE_PATH(shader.vert), FILE_PATH(shader.frag));
}

Engine::~Engine() {
	glfwDestroyWindow(m_window);
}

void Engine::Setup()
{
	SetupJolt();
	glfwInit();
}

void Engine::Dismantle()
{
	DismantleJolt();
}

void Engine::Update(double deltaTime)
{
	physics_server.Update(deltaTime);

	ProcessInput(m_window);

	glfwGetCursorPos(m_window, &mousePos.x, &mousePos.y);

	double sensitivity = 2.0;
	mouseMovement = (mousePos - lastMousePos) * sensitivity * deltaTime;
	lastMousePos = mousePos;

	if (deltaTime > 0.0) {
		glfwSetWindowTitle(m_window, (std::string("FPS: ") + std::to_string(static_cast<int>(1.0 / deltaTime))).c_str());
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (current_camera) {
		mat4 view = current_camera->GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(view));
		mat4 projection = current_camera->GetProjectionMatrix(static_cast<float>(InitWidth) / static_cast<float>(InitHeight));
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	for (auto obj : process_objects) {
		obj->Process(deltaTime);
	}

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

GLFWwindow* Engine::CreateWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(InitWidth, InitHeight, "Title", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fmt::println("Failed to initialize GLAD");
		// TODO: May need to return error or throw exeption.
		return nullptr;
	}
	glViewport(0, 0, InitWidth, InitHeight);
	glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);
	return window;
}
