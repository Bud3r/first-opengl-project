#pragma once


#include "process_object.h"
#include "engine.h"


class GameProcessObject : public ProcessObject
{
public:
    GameProcessObject();
	GameProcessObject(Engine* engine);
	~GameProcessObject();
	virtual void Process(double deltaTime) override;
private:
    Camera camera;
    PhysicsBody ak_body;
    PhysicsBody floor_body;
};


GameProcessObject::GameProcessObject(Engine* p_engine) {
    Engine* engine = p_engine;
}


void GameProcessObject::Process(double deltaTime) {
    camera.Rotation.y += static_cast<float>(engine->mouseMovement.x);
    camera.Rotation.x = clamp(camera.Rotation.x + static_cast<float>(engine->mouseMovement.y), glm::radians(-89.0f), glm::radians(89.0f));
    
    vec4 movement(
        engine->movementInput.x * static_cast<float>(deltaTime),
        0.0f,
        engine->movementInput.y * static_cast<float>(deltaTime),
        1.0f
    );

    auto ak_body_pos = ak_body.GetPosition();
    auto ak_body_rot = ak_body.GetRotation().GetEulerAngles();
    //ak_model.m_position = vec3(ak_body_pos.GetX(), ak_body_pos.GetY(), ak_body_pos.GetZ());
    //ak_model.m_rotation = vec3(ak_body_rot.GetX(), ak_body_rot.GetY() + 0.9f, ak_body_rot.GetZ());

    camera.Move(movement);
}