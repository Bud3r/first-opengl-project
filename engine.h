#pragma once

#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "helper.h"
#include "shader_program.h"
#include "camera.h"
#include "time.h"
#include "physics_body.h"
#include "game_object.h"
#include "resource_loader.h"

constexpr int UBO_BINDING = 0;
constexpr int INIT_WINDOW_WIDTH = 800;
constexpr int INIT_WINDOW_HEIGHT = 600;
constexpr int ERR_LOG_SIZE = 512;

void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Engine
{
public:
	Engine();
	~Engine();
	static void Setup();
	static void Dismantle();
	void Update(double deltaTime);
	void AddProcessObject(GameObject* process_object);
	PhysicsServer physics_server;
	ResourceLoader resource_loader;
	glm::vec2 movementInput;
	glm::dvec2 mouseMovement;
	glm::dvec2 mousePos;
	Camera* current_camera;
	std::vector<GameObject*> process_objects;
	const ShaderProgram& GetDefaultShaderProgram() const;
private:
	ShaderProgram default_shader_program;
	void ProcessInput(GLFWwindow* window);
	GLFWwindow* CreateWindow();
	GLFWwindow* m_window;

	unsigned int UBO;
	glm::dvec2 lastMousePos;
	double oldTime;
};