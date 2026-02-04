#pragma once


class ProcessObject;


class GameProcessObject : public ProcessObject
{
public:
    GameProcessObject();
    virtual void AddedToEngine() override;
	virtual void Process(double deltaTime) override;
private:
    Camera camera = Camera();
    PhysicsBody ak_body;
    PhysicsBody floor_body;
    Model* ak_mesh;
};

GameProcessObject::GameProcessObject() { }

void GameProcessObject::AddedToEngine() {
    engine->current_camera = &camera;
    ak_body = PhysicsBody(&engine->physics_server, BodyCreationSettings(
        new BoxShape(Vec3Arg(1.0f, 1.0f, 1.0f)), 
        RVec3Arg(0.0_r, 0.0_r, 0.0_r), 
        QuatArg::sIdentity(), 
        EMotionType::Dynamic, 
        Layers::MOVING)
    );
    ak_mesh = engine->resource_loader.Load<Model>(FILE_PATH(ak47.glb));
    ak_mesh->m_modulate;
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
    ak_mesh->Draw(engine->test);
    ak_mesh->m_position = vec3(ak_body_pos.GetX(), ak_body_pos.GetY(), ak_body_pos.GetZ());
    ak_mesh->m_rotation = vec3(ak_body_rot.GetX(), ak_body_rot.GetY() + 0.9f, ak_body_rot.GetZ());

    camera.Move(movement);
}