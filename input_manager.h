#pragma once

#include "GLFW/glfw3.h"
#include <vector>

typedef void* (*InputFunc)(InputEvent&);

enum InputEventType
{
	KEYBOARD,
	MOUSE_BUTTON,
	NONE,
};

enum PressType
{
	PRESSED = 1,
	JUST_PRESSED = 2,
	RELEASED = 4,
	JUST_RELEASED = 8,
};

struct InputEvent
{
	InputEventType m_type = InputEventType::NONE;
	int m_key_label;
};

void init(GLFWwindow* p_window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);

InputFunc input_callback[];
bool m_mouse_button_pressed[GLFW_MOUSE_BUTTON_LAST];
bool m_key_pressed[GLFW_KEY_LAST];
