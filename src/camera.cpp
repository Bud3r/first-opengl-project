#include "camera.h"

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + glm::vec3(cos(Rotation.y), 0.0f, sin(Rotation.y)), Up);
}


glm::mat4 Camera::GetProjectionMatrix(float ratio) {
	return glm::perspective(FOV, ratio, NearClip, FarClip);
}