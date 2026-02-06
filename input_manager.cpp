#include "input_manager.h"

std::map<std::string, Action> actions;
std::list<GameObject*> input_callback_game_objects;

bool m_mouse_button_pressed[GLFW_MOUSE_BUTTON_LAST];
bool m_key_pressed[GLFW_KEY_LAST];

void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void _mouse_callback(GLFWwindow * window, int button, int action, int mods);


void Input_init(GLFWwindow* p_window) {
	glfwSetKeyCallback(p_window, _key_callback);
	glfwSetMouseButtonCallback(p_window, _mouse_callback);
}

glm::vec3 Input_get_action_3d(std::string action, InputEventType event_type)
{
	auto k = actions[action].keys[event_type];

	if (event_type == InputEventType::KEYBOARD) {
		return glm::vec3(
			static_cast<float>(m_key_pressed[k[0]]) - static_cast<float>(m_key_pressed[k[1]]),
			static_cast<float>(m_key_pressed[k[2]]) - static_cast<float>(m_key_pressed[k[3]]),
			static_cast<float>(m_key_pressed[k[4]]) - static_cast<float>(m_key_pressed[k[5]])
		);
	}

	return glm::vec3(0.0f);
}

glm::vec2 Input_get_action_2d(std::string action, InputEventType event_type)
{
	auto k = actions[action].keys[event_type];

	if (event_type == InputEventType::KEYBOARD) {
		return glm::vec2(
			static_cast<float>(m_key_pressed[k[0]]) - static_cast<float>(m_key_pressed[k[1]]), 
			static_cast<float>(m_key_pressed[k[2]]) - static_cast<float>(m_key_pressed[k[3]])
		);
	}

	return glm::vec2(0.0f);
}

float Input_get_action_1d(std::string action, InputEventType event_type)
{
	auto k = actions[action].keys[event_type];

	if (event_type == InputEventType::KEYBOARD) {
		if (k.size() == 1) {
			return m_key_pressed[k[0]];
		}
		else if (k.size() == 2) {
			return static_cast<float>(m_key_pressed[k[0]]) - static_cast<float>(m_key_pressed[k[1]]);
		}
	}

	return 0.0f;
}

bool Input_get_action_binary(std::string action, InputEventType event_type)
{
	auto k = actions[action].keys[event_type];

	if (event_type == InputEventType::KEYBOARD) {
		if (k.size() == 1) {
			return m_key_pressed[k[0]];
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
void Input_add_action(std::string action, InputEventType type, std::vector<int> input_idxs) {
	if (!actions.contains(action)) {
		actions[action] = Action();
	}

	if (!actions[action].keys.contains(type)) {
		actions[action].keys[type] = std::vector<int>(6);
	}
	
	actions[action].keys[type] = input_idxs;
}

void Input_add_callback(GameObject* object) {
	input_callback_game_objects.push_back(object);
}

void Input_remove_callback(GameObject* object) {
	input_callback_game_objects.remove(object);
}

void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressed = GLFW_RELEASE != action;
	float strength = 0.0f;
	PressFlags press_type = PressFlags::NONE;

	if (m_key_pressed[key] != pressed) {
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

	for (auto func : input_callback_game_objects) {
		func->process_input(event);
	}

	m_key_pressed[key] = pressed;
}

void _mouse_callback(GLFWwindow* window, int button, int action, int mods) {

}