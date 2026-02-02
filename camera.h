#pragma once


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


using namespace glm;


class Camera
{
public:
	/// <summary>
	///  In radians
	/// </summary>
	float FOV = radians(45.0f);
	float NearClip = 0.1f;
	float FarClip = 100.0f;
	vec3 Position = vec3(0.0f);
	vec3 Rotation = vec3(0.0f);

	mat4 GetViewMatrix();
	mat4 GetProjectionMatrix(float ratio);
};


mat4 Camera::GetViewMatrix() {
	mat4 view = identity<mat4>();
	view = rotate(view, Rotation.x, vec3(1.0f, 0.0f, 0.0f));
	view = rotate(view, Rotation.y, vec3(0.0f, 1.0f, 0.0f));
	view = rotate(view, Rotation.z, vec3(0.0f, 0.0f, 1.0f));
	view = translate(view, Position);
	return view;
}


mat4 Camera::GetProjectionMatrix(float ratio) {
	mat4 projection = glm::perspective(FOV, ratio, NearClip, FarClip);
	return projection;
}