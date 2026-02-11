#include "physics_model_game_object.h"


PhysicsModelGameObject::PhysicsModelGameObject(Model* p_model, BodyCreationSettings* p_body_creation_settings) {
    m_model = p_model;
    m_initial_body_creation_setting = p_body_creation_settings;
}

void PhysicsModelGameObject::Process(double deltaTime)
{
    auto position = m_body.GetPosition();
    auto rotation = m_body.GetRotation().GetEulerAngles();

    m_model->Draw(
        engine->GetDefaultShaderProgram(), 
        Model::create_model_matrix(
            glm::vec3(position.GetX(), position.GetY(), position.GetZ()),
            glm::vec3(rotation.GetX(), rotation.GetY(), rotation.GetZ())),
        modulate
    
    );
}

void PhysicsModelGameObject::AddedToEngine()
{
    m_body.Init(&engine->physics_server, *m_initial_body_creation_setting);
    m_initial_body_creation_setting = nullptr;
}

PhysicsBody& PhysicsModelGameObject::GetBody()
{
    return m_body;
}


