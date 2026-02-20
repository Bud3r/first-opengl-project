#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const {
	return LookAt(position, rotation, kUp);
}

glm::mat4 Camera::GetProjectionMatrix(float ratio) const {
	return glm::perspective(fov, ratio, near_clip, far_clip);
}