#include "physics_model_game_object.h"


PhysicsModelGameObject::PhysicsModelGameObject(std::shared_ptr<Model> model, JPH::BodyCreationSettings* body_creation_settings) {
    model_ = model;
    initial_body_creation_setting_ = body_creation_settings;
}

void PhysicsModelGameObject::Process(double deltaTime)
{
    auto position = body_.GetPosition();
    auto rotation = body_.GetRotation().GetEulerAngles();

    model_->Draw(
        engine->GetDefaultShaderProgram(), 
        Model::GetModelMatrix(
            glm::vec3(position.GetX(), position.GetY(), position.GetZ()),
            glm::vec3(rotation.GetX(), rotation.GetY(), rotation.GetZ())),
        modulate
    
    );
}

void PhysicsModelGameObject::AddedToEngine()
{
    body_.Init(&engine->physics_server, *initial_body_creation_setting_);
    initial_body_creation_setting_ = nullptr;
}

PhysicsBody& PhysicsModelGameObject::GetBody()
{
    return body_;
}


