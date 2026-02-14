#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const {
	glm::vec3 front(glm::cos(rotation.y), 
		-glm::sin(rotation.x), 
		sin(rotation.y) * glm::cos(rotation.x));
	return glm::lookAt(position, position + front, kUp);
}

glm::mat4 Camera::GetProjectionMatrix(float ratio) const {
	return glm::perspective(fov, ratio, near_clip, far_clip);
}