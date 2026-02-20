#include "input_manager.h"


glm::vec3 InputManager::GetAction3d(std::string action, InputEventType event_type)
{
	auto k = actions_[action].keys[event_type];

	if (event_type == InputEventType::kKeyboard) {
		return glm::vec3(
			static_cast<float>(key_pressed_[k[0]]) - static_cast<float>(key_pressed_[k[1]]),
			static_cast<float>(key_pressed_[k[2]]) - static_cast<float>(key_pressed_[k[3]]),
			static_cast<float>(key_pressed_[k[4]]) - static_cast<float>(key_pressed_[k[5]])
		);
	}

	return glm::vec3(0.0f);
}

glm::vec2 InputManager::GetAction2d(std::string action, InputEventType event_type)
{
	auto k = actions_[action].keys[event_type];

	if (event_type == InputEventType::kKeyboard) {
		return glm::vec2(
			static_cast<float>(key_pressed_[k[0]]) - static_cast<float>(key_pressed_[k[1]]), 
			static_cast<float>(key_pressed_[k[2]]) - static_cast<float>(key_pressed_[k[3]])
		);
	}

	return glm::vec2(0.0f);
}

float InputManager::GetAction1d(std::string action, InputEventType event_type)
{
	auto k = actions_[action].keys[event_type];

	if (event_type == InputEventType::kKeyboard) {
		if (k.size() == 1) {
			return key_pressed_[k[0]];
		}
		else if (k.size() == 2) {
			return static_cast<float>(key_pressed_[k[0]]) - static_cast<float>(key_pressed_[k[1]]);
		}
	}

	return 0.0f;
}

bool InputManager::GetActionBinary(std::string action, InputEventType event_type)
{
	auto k = actions_[action].keys[event_type];

	if (event_type == InputEventType::kKeyboard) {
		if (k.size() == 1) {
			return key_pressed_[k[0]];
		}
	}

	return false;
}
/// <summary>
/// 
/// </summary>
/// <param name="action"></param>
/// <param name="type"></param>
/// <param name="input_idxs"> {x, -x, y, -y, z, -z} </param>
void InputManager::AddAction(std::string action, InputEventType type, std::vector<int> input_idxs) {
	if (!actions_.contains(action)) {
		actions_[action] = Action();
	}

	if (!actions_[action].keys.contains(type)) {
		actions_[action].keys[type] = std::vector<int>(6);
	}
	
	actions_[action].keys[type] = input_idxs;
}

void InputManager::AddCallback(GameObject* object) {
	input_callback_game_objects_.push_back(object);
}

void InputManager::RemoveCallback(GameObject* object) {
	input_callback_game_objects_.remove(object);
}

void InputManager::SetMouseMode(MouseMode mouse_mode) {
	switch (mouse_mode)
	{
		case MouseMode::kNormal:
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
		case MouseMode::kHidden:
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
		case MouseMode::kCaptured:
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		break;
		case MouseMode::kDisabled:
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
		default:
		break;
	}
}

MouseMode InputManager::GetMouseMode() {
	switch (glfwGetInputMode(glfw_window_, GLFW_CURSOR))
	{
		case GLFW_CURSOR_NORMAL:
		return MouseMode::kNormal;
		break;
		case GLFW_CURSOR_HIDDEN:
		return MouseMode::kHidden;
		break;
		case GLFW_CURSOR_CAPTURED:
		return MouseMode::kCaptured;
		break;
		case GLFW_CURSOR_DISABLED:
		return MouseMode::kDisabled;
		break;
		default:
		return MouseMode();
		break;
	} 
}


PressFlags InputManager::GetPressType(int action, bool was_pressed) {
	bool pressed = GLFW_RELEASE != action;
	PressFlags press_type = PressFlags::kNone;

	if (was_pressed != pressed) {
		press_type = press_type | PressFlags::kJust;
	}

	switch (action)
	{
		case GLFW_PRESS:
		press_type = press_type | PressFlags::kPressed;
		break;
		case GLFW_RELEASE:
		press_type = press_type | PressFlags::kReleased;
		break;
		case GLFW_REPEAT:
		press_type = press_type | PressFlags::kRepeat;
		break;
		default:
		break;
	}

	return press_type;
}


void InputManager::KeyCallback(int key, int scancode, int action, int mods) {
	bool pressed = GLFW_RELEASE != action;
	PressFlags press_type = GetPressType(action, key_pressed_[key]);
	
	InputEvent event{
		InputEventType::kKeyboard,
		press_type,
	};

	event.key = key;

	PropagateInputEvent(event);

	key_pressed_[key] = pressed;
}

void InputManager::MouseButtonCallback(int button, int action, int mods) {
	bool pressed = GLFW_RELEASE != action;
	PressFlags press_type = GetPressType(action, mouse_button_pressed_[button]);
	
	InputEvent event{
		InputEventType::kMouseButton,
		press_type,
	};

	event.button = button;

	PropagateInputEvent(event);

	mouse_button_pressed_[button] = pressed;
}

void InputManager::MousePosCallback(double xpos, double ypos) {
	glm::vec2 mouse_pos(xpos, ypos);

	glm::vec2 movement = mouse_pos - last_mouse_pos_;

	InputEvent event{
		InputEventType::kMouseMotion,
		PressFlags::kNone,
	};

	event.mouse_movement_x = movement.x;
	// When moving the mouse down it's positive relative to the screen.
	// Inverting it will make is clearer for 3d space.
	event.mouse_movement_y = -movement.y;

	PropagateInputEvent(event);

	last_mouse_pos_ = mouse_pos;
}

void InputManager::SetRawMouseMotion(bool turned_on) {
	glfwSetInputMode(glfw_window_, GLFW_RAW_MOUSE_MOTION, static_cast<int>(turned_on));
}

bool InputManager::GetRawMouseMotion() {
	return static_cast<bool>(glfwGetInputMode(glfw_window_, GLFW_RAW_MOUSE_MOTION));
}

void InputManager::PropagateInputEvent(InputEvent& event) {
	for (auto func : input_callback_game_objects_) {
		func->ProcessInput(event);
	}
}
