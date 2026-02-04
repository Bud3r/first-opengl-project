#pragma once


#include "physics_model_game_object.h"


class GameObject;


class MainGameObject : public GameObject
{
public:
    virtual void AddedToEngine() override;
	virtual void Process(double deltaTime) override;
private:
    Camera camera = Camera();
    PhysicsBody floor_body;
    PhysicsModelGameObject* game_object;
};

