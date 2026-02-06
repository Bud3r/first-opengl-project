#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "game_object.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <list>

enum class InputEventType
{
	KEYBOARD,
	MOUSE_BUTTON,
	JOY_STICK,
	JOY_BUTTON,
	NONE,
};

enum class AxisMode
{
	BINARY,
	AXIS_1D,
	AXIS_2D,
	AXIS_3D,
};

enum class PressFlags
{
	NONE = 0,
	PRESSED = 1,
	RELEASED = 2,
	REPEAT = 4,
	JUST = 8,
	JUST_PRESSED = 1 | 8,
	JUST_RELEASED = 2 | 8,
};

inline PressFlags operator|(PressFlags a, PressFlags b)  {
	return static_cast<PressFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline PressFlags operator&(PressFlags a, PressFlags b) {
	return static_cast<PressFlags>(static_cast<int>(a) & static_cast<int>(b));
}

struct InputEvent {
	InputEventType m_type = InputEventType::NONE;
	PressFlags m_press_flags;
	int m_key_label;
	float strength;

	inline bool Equal(const InputEvent& p_other) {
		return false;
	}
};

struct Action {
	static constexpr int ACTION_MAX = 6;
	std::map<InputEventType, std::vector<int>> keys;
	AxisMode m_axis;
};

glm::vec3 Input_get_action_3d(std::string action, InputEventType event_type);
glm::vec2 Input_get_action_2d(std::string action, InputEventType event_type);
float Input_get_action_1d(std::string action, InputEventType event_type);
bool Input_get_action_binary(std::string action, InputEventType event_type);
void Input_add_action(std::string action, InputEventType type, std::vector<int> input_idxs);
void Input_init(GLFWwindow* p_window);
void Input_add_callback(GameObject* object);
void Input_remove_callback(GameObject* object);