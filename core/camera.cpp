#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position, position + glm::vec3(cos(rotation.y), 0.0f, sin(rotation.y)), kUp);
}

glm::mat4 Camera::GetProjectionMatrix(float ratio) const {
	return glm::perspective(fov, ratio, near_clip, far_clip);
}