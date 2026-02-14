#pragma once


#include <GLFW/glfw3.h>





class Window {
 public:
	 Window(int width, int height) {
		 glfw_window_ = glfwCreateWindow(width, height);
		 glfwCreateWindow()
	 };
	 ~Window() {
		 glfwDestroyWindow(glfw_window_);
	 };
	 void SetTitle(const char* title) {
		 glfwSetWindowTitle(glfw_window_, title)
	 };
	 const char* GetTitle() {
		 glfwGetWindowTitle(glfw_window_);
	 };
 private:
	 GLFWwindow* glfw_window_;


};