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
	kNone,
	kKeyboard,
	kMouseButton,
	kMouseMotion,
	kJoyMotion,
	kJoyButton,
};

enum class AxisMode {
	kBinary,
	kAxis1d,
	kAxis2d,
	kAxis3d,
};

enum class PressFlags {
	kNone = 0,
	kPressed = 1,
	kReleased = 2,
	kRepeat = 4,
	kJust = 8,
	kJustPressed = 1 | 8,
	kJustReleased = 2 | 8,
};

inline PressFlags operator|(PressFlags a, PressFlags b)  {
	return static_cast<PressFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline PressFlags operator&(PressFlags a, PressFlags b) {
	return static_cast<PressFlags>(static_cast<int>(a) & static_cast<int>(b));
}

struct InputEvent {
	InputEventType type = InputEventType::kNone;
	PressFlags press_flags;
	union {
		int key;
		int button;
		float mouse_movement_x;
	};
	union {
		int key_label;
		float strength;
		float mouse_movement_y;
	};
};

struct Action {
	static constexpr int kActionSizeMax = 6;
	std::map<InputEventType, std::vector<int>> keys;
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

	void SetGlfwWindow(GLFWwindow* window) {
		glfw_window_ = window;
		double cursor_x;
		double cursor_y;
		glfwGetCursorPos(window, &cursor_x, &cursor_y);
		last_mouse_pos_ = glm::vec2(cursor_x, cursor_y);
	}

private:
	void PropagateInputEvent(InputEvent& event);
	PressFlags GetPressType(int action, bool was_pressed);

	std::map<std::string, Action> actions_;
	std::list<GameObject*> input_callback_game_objects_;
	std::array<bool, GLFW_MOUSE_BUTTON_LAST> mouse_button_pressed_ = { false };
	std::array<bool, GLFW_KEY_LAST> key_pressed_ = { false };
	glm::vec2 last_mouse_pos_ = glm::vec2(0.0f);
	GLFWwindow* glfw_window_ = nullptr;
};