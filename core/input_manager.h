#pragma once

#include <string>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <functional>
#include <list>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "game_object.h"


enum class MouseMode {
	kNormal,
	kHidden,
	kCaptured,
	kDisabled,
};

enum class InputEventType {
	KEYBOARD,
	MOUSE_BUTTON,
	MOUSE_MOTION,
	JOY_STICK,
	JOY_BUTTON,
	NONE,
};

enum class AxisMode {
	BINARY,
	AXIS_1D,
	AXIS_2D,
	AXIS_3D,
};

enum class PressFlags {
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

class InputManager {
public:
	glm::vec3 GetAction3d(std::string action, InputEventType event_type);
	glm::vec2 GetAction2d(std::string action, InputEventType event_type);
	float GetAction1d(std::string action, InputEventType event_type);
	bool GetActionBinary(std::string action, InputEventType event_type);
	void AddAction(std::string action, InputEventType type, std::vector<int> input_idxs);
	void AddCallback(GameObject* object);
	void RemoveCallback(GameObject* object);
	
	void KeyCallback(int key, int scancode, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);
	void MousePosCallback(double xpos, double ypos);

	void SetMouseMode(MouseMode mouse_mode);
	MouseMode GetMouseMode();

	void SetRawMouseMotion(bool turned_on);
	bool GetRawMouseMotion();

	glm::vec2 GetLastMousePos() {
		return last_mouse_pos_;
	}

	GLFWwindow* glfw_window;
private:
	std::map<std::string, Action> actions_;
	std::list<GameObject*> input_callback_game_objects_;
	std::array<bool, GLFW_MOUSE_BUTTON_LAST> mouse_button_pressed_ = { false };
	std::array<bool, GLFW_KEY_LAST> key_pressed_ = { false };
	glm::vec2 last_mouse_pos_;
};