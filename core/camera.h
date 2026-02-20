#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "helper.h"

class Camera
{
public:
	static constexpr glm::vec3 kUp = glm::vec3(0.0f, 1.0f, 0.0f);
	/// <summary>
	///  In radians
	/// </summary>
	float fov = glm::radians(45.0f);
	float near_clip = 0.1f;
	float far_clip = 100.0f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float ratio) const;
};
