#include "camera.h"


void Camera::Move(glm::vec3 p_movement) {
	glm::vec3 camera_front = glm::normalize(glm::vec3(
		cos(Rotation.y) * cos(Rotation.x),
		-sin(Rotation.x),
		sin(Rotation.y) * cos(Rotation.x)
	));

	glm::vec3 camera_right = glm::normalize(glm::cross(Up, camera_front));

	Position -= camera_front * p_movement.z;
	Position -= camera_right * p_movement.x;
}

glm::mat4 Camera::GetViewMatrix() {
	glm::mat4 view = glm::identity<glm::mat4>();
	view = rotate(view, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	view = rotate(view, Rotation.y + glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = rotate(view, Rotation.z, glm::vec3(0.0f, 0.0f, -1.0f));
	view = translate(view, Position);
	return view;
}


glm::mat4 Camera::GetProjectionMatrix(float ratio) {
	glm::mat4 projection = glm::perspective(FOV, ratio, NearClip, FarClip);
	return projection;
}