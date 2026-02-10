#pragma once


#include "physics_model_game_object.h"
#include "game_objects/player_game_object.h"

class GameObject;


class MainGameObject : public GameObject
{
public:
    virtual void AddedToEngine() override;
private:
    PhysicsModelGameObject* floor_game_object;
    PhysicsModelGameObject* ak_game_object;
    PlayerGameObject* player_game_object;
};

