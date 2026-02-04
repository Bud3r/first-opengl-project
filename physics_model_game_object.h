#pragma once


#include "engine.h"


class PhysicsModelGameObject : public GameObject
{
public:
	PhysicsModelGameObject(Model* model, BodyCreationSettings* body_settings);
	PhysicsBody& GetBody();
	virtual void AddedToEngine() override;
	virtual void Process(double deltaTime) override;
private:
	Model* m_model;
	PhysicsBody m_body;
	BodyCreationSettings* m_initial_body_creation_setting;
};