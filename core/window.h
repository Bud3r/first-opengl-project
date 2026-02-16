#pragma once

#include <array>

#include "glad/glad.h"

#include "input_manager.h"

class Window {
 public:
	 Window() { };
	 Window(int width, int height, const char* title);
	 ~Window();

	 void SetTitle(const char* title);
	 const char* GetTitle();

	 // TODO: Implement size functions.
	 void SetSize(glm::vec2 size) { };
	 glm::vec2 GetSize(int width, int height) { return glm::vec2(0.0f); };

	 GLFWwindow* GetGlfwWindow() { return glfw_window_; }

	 int ShouldClose() {
		 return glfwWindowShouldClose(glfw_window_); 
	 }
	 void Close();
	 void MakeContextCurrent();

	 bool auto_accept_quit = true;
	 InputManager input_manager;
 private:
	 static std::map<GLFWwindow*, Window*> glfw_window_to_object;
	 static void GlobalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	 static void GlobalMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	 static void GlobalMousePosCallback(GLFWwindow* window, double xpos, double ypos);
	 static void GlobalFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	 static void GlobalWindowCloseCallback(GLFWwindow* window);

	 GLFWwindow* glfw_window_;
};