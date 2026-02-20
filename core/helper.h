#pragma once

#include <string>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "Jolt/Jolt.h"

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

std::string get_real_file_path(const std::string& path);


inline JPH::Vec3 GlmVecToJolt(glm::vec3 vec) {
	return { vec.x, vec.y, vec.z };
}

inline glm::vec3 GetFront(glm::vec3 euler) {
	return glm::vec3(
		-glm::cos(euler.x) * glm::sin(euler.y),
		glm::sin(euler.x),
		-glm::cos(euler.x) * glm::cos(euler.y)
	);
}

inline glm::mat4 LookAtModel(glm::vec3 euler, glm::vec3 up) {
	glm::vec3 front(
		-glm::cos(euler.x) * glm::sin(euler.y),
		glm::sin(euler.x),
		-glm::cos(euler.x) * glm::cos(euler.y)
	);

	return glm::lookAtRH(glm::vec3(0.0f), front, up);
}

inline glm::mat4 LookAt(glm::vec3 euler, glm::vec3 up) {
	glm::vec3 front(
		-glm::cos(euler.x) * glm::sin(euler.y),
		glm::sin(euler.x),
		-glm::cos(euler.x) * glm::cos(euler.y)
	);

	front.y = -front.y;
	front.z = -front.z;

	return glm::lookAtRH(glm::vec3(0.0f), front, up);
}

inline glm::mat4 LookAt(glm::vec3 position, glm::vec3 euler, glm::vec3 up) {
	glm::vec3 front(
		-glm::cos(euler.x) * glm::sin(euler.y),
		glm::sin(euler.x),
		-glm::cos(euler.x) * glm::cos(euler.y)
	);

	return glm::lookAtRH(position, position + front, up);
}