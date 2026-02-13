#pragma once


#include "physics_model_game_object.h"
#include "player_game_object.h"


class MainGameObject : public GameObject
{
public:
    virtual void AddedToEngine() override;
    virtual void Process(double delta) override;
private:
    PhysicsModelGameObject* floor_game_object;
    PhysicsModelGameObject* ak_game_object;
    PlayerGameObject* player_game_object;
    std::shared_ptr<Model> model;
};

