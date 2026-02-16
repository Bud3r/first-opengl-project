#include "input_manager.h"


glm::vec3 InputManager::GetAction3d(std::string action, InputEventType event_type)
{
	auto k = actions_[action].keys[event_type];

	if (event_type == InputEventType::KEYBOARD) {
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

	if (event_type == InputEventType::KEYBOARD) {
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

	if (event_type == InputEventType::KEYBOARD) {
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

	if (event_type == InputEventType::KEYBOARD) {
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
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
		case MouseMode::kHidden:
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
		case MouseMode::kCaptured:
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		break;
		case MouseMode::kDisabled:
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
		default:
		break;
	}
}

MouseMode InputManager::GetMouseMode() {
	switch (glfwGetInputMode(glfw_window, GLFW_CURSOR))
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

void InputManager::KeyCallback(int key, int scancode, int action, int mods) {
	bool pressed = GLFW_RELEASE != action;
	float strength = 0.0f;
	PressFlags press_type = PressFlags::NONE;

	//if (key == Input_key_mouse_enable) {
	//	if (action == GLFW_PRESS) {
	//		GLFWwindow* window = glfwGetCurrentContext();
	//		int cursor_mode = glfwGetInputMode(window, GLFW_CURSOR);
	//		if (cursor_mode == GLFW_CURSOR_DISABLED) {
	//			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//		}
	//		else {
	//			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//		}
	//	}
	//}

	if (key_pressed_[key] != pressed) {
		press_type = press_type | PressFlags::JUST;
	}

	switch (action)
	{
	case GLFW_PRESS:
		press_type = press_type | PressFlags::PRESSED;
		strength = 1.0f;
		break;
	case GLFW_RELEASE:
		press_type = press_type | PressFlags::RELEASED;
		strength = 1.0f;
		break;
	case GLFW_REPEAT:
		press_type = press_type | PressFlags::REPEAT;
		strength = 1.0f;
		break;
	default:
		break;
	}

	InputEvent event{
		InputEventType::KEYBOARD,
		press_type,
		key,
	};

	for (auto func : input_callback_game_objects_) {
		func->ProcessInput(event);
	}

	key_pressed_[key] = pressed;
}

void InputManager::MouseButtonCallback(int button, int action, int mods) {

}

void InputManager::MousePosCallback(double xpos, double ypos) {
	glm::vec2 mouse_pos(xpos, ypos);

	glm::vec2 movement = mouse_pos - last_mouse_pos_;

	last_mouse_pos_ = mouse_pos;
}

void InputManager::SetRawMouseMotion(bool turned_on) {
	glfwSetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION, static_cast<int>(turned_on));
}

bool InputManager::GetRawMouseMotion() {
	return static_cast<bool>(glfwGetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION));
}
