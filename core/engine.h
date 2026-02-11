#pragma once

#include <fmt/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "imgui_impl_opengl3.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "physics_server.h"
#include "helper.h"
#include "shader_program.h"
#include "camera.h"
#include "time.h"
#include "physics_body.h"
#include "game_object.h"
#include "resource_loader.h"
#include "input_manager.h"

constexpr int UBO_BINDING = 0;
constexpr int INIT_WINDOW_WIDTH = 800;
constexpr int INIT_WINDOW_HEIGHT = 600;
constexpr int ERR_LOG_SIZE = 512;

inline void print_vec(glm::vec4 vec) {
	std::printf("(%f, %f, %f, %f)\n", vec.x, vec.y, vec.z, vec.w);
}

inline void print_vec(glm::vec3 vec) {
	std::printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z);
}

inline void print_vec(glm::vec2 vec) {
	std::printf("(%f, %f)\n", vec.x, vec.y);
}

inline glm::vec3 RVec3tovec3(JPH::RVec3 vec) {
	return glm::vec3(vec.GetX(), vec.GetY(), vec.GetZ());
}

inline glm::vec3 Vec3tovec3(JPH::Vec3 vec) {
	return glm::vec3(vec.GetX(), vec.GetY(), vec.GetZ());
}

void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Engine
{
public:
	Engine();
	~Engine();
	void Update(double deltaTime);
	void add_game_object(GameObject* process_object);
	void start();
	template<typename T>
	void set_root_game_object() {
		add_game_object(new T);
	}

	PhysicsServer physics_server;
	ResourceLoader resource_loader;

	Camera* current_camera = nullptr;
	std::vector<GameObject*> process_objects;

	glm::vec2 movementInput;
	glm::dvec2 mouseMovement;
	glm::dvec2 mousePos;

	const ShaderProgram& GetDefaultShaderProgram() const;
private:
	GLFWwindow* CreateWindow();

	ShaderProgram default_shader_program;

	GLFWwindow* m_window;
	glm::dvec2 lastMousePos;

	unsigned int UBO;
	double oldTime;
};