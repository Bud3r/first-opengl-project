#pragma once

#include "physics_model_game_object.h"
#include "engine.h"
#include <glm/glm.hpp>


class PlayerGameObject : public GameObject
{
public:
	virtual void AddedToEngine() override;
	virtual void Process(double delta_time) override;
	virtual void ProcessInput(InputEvent& input_event) override;
private:
	bool noclip = false;
	JPH::Vec3 up = JPH::Vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera_rotation = glm::vec3(0.0f);
	float vertical_velocity = 0.0f;
	
	std::shared_ptr<Model> ak_model;
	Camera camera;
	PhysicsBody body;
};