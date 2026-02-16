#include <iostream>
#include <map>

#include "window.h"

std::map<GLFWwindow*, Window*> Window::glfw_window_to_object = {};

void Window::MakeContextCurrent() {
	glfwMakeContextCurrent(glfw_window_);
}

void Window::GlobalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	glfw_window_to_object[window]->input_manager.KeyCallback(key, scancode, action, mods);
}
void Window::GlobalMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	glfw_window_to_object[window]->input_manager.MouseButtonCallback(button, action, mods);
}
void Window::GlobalMousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	glfw_window_to_object[window]->input_manager.MousePosCallback(xpos, ypos);
}
void Window::GlobalFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glfw_window_to_object[window]->SetSize(glm::vec2(width, height));
}
void Window::GlobalWindowCloseCallback(GLFWwindow* window) {
	Window* object = glfw_window_to_object[window];
	if (object->auto_accept_quit) {
		object->Close();
	}
}

Window::Window(int width, int height, const char* title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	MakeContextCurrent();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
	}

	input_manager.glfw_window = glfw_window_;

	glfw_window_to_object[glfw_window_] = this;

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(glfw_window_, Window::GlobalFramebufferSizeCallback);
	glfwSetKeyCallback(glfw_window_, Window::GlobalKeyCallback);
	glfwSetMouseButtonCallback(glfw_window_, Window::GlobalMouseButtonCallback);
	glfwSetCursorPosCallback(glfw_window_, Window::GlobalMousePosCallback);
	glfwSetWindowCloseCallback(glfw_window_, Window::GlobalWindowCloseCallback);
}

Window::~Window() {
	Close();
}

void Window::SetTitle(const char* title) {
	glfwSetWindowTitle(glfw_window_, title);
}

const char* Window::GetTitle() {
	return glfwGetWindowTitle(glfw_window_);
}

void Window::Close() {
	glfwDestroyWindow(glfw_window_);
	glfw_window_to_object.erase(glfw_window_);
	glfw_window_ = nullptr;
}
