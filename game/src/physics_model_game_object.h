#pragma once


#include "engine.h"
#include "model.h"


class PhysicsModelGameObject : public GameObject
{
public:
	PhysicsModelGameObject(std::shared_ptr<Model> model, JPH::BodyCreationSettings* body_settings);
	PhysicsBody& GetBody();
	virtual void AddedToEngine() override;
	virtual void Process(double deltaTime) override;
	glm::vec4 modulate = glm::vec4(1.0);
protected:
	std::shared_ptr<Model> model_;
	PhysicsBody body_;
private:
	JPH::BodyCreationSettings* initial_body_creation_setting_;
};