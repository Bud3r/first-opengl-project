#pragma once


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


class Camera
{
public:
	static constexpr glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	/// <summary>
	///  In radians
	/// </summary>
	float FOV = glm::radians(45.0f);
	float NearClip = 0.1f;
	float FarClip = 100.0f;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);

	void Move(glm::vec3 p_movement);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float ratio);
};
