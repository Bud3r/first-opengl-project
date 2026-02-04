#include "input_manager.h"


void init(GLFWwindow* p_window) {
	glfwSetKeyCallback(p_window, key_callback);
	glfwSetMouseButtonCallback(p_window, mouse_callback);
	//glfwSetScrollCallback
	//glfwSetJoystickCallback
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (input_callback)
	m_key_pressed[key] = action;
	GLFW_KEY_0;
	GLFW_MOUSE_BUTTON_8;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {

}