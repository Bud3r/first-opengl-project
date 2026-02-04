#include "main_game_object.h"


void MainGameObject::AddedToEngine() {
    engine->current_camera = &camera;
    engine->physics_server.m_physics_system.SetGravity(Vec3Arg(0.0, -0.1, 0.0));
    auto body_settings = BodyCreationSettings(
        new BoxShape(Vec3Arg(1.0f, 1.0f, 1.0f)),
        RVec3Arg(0.0_r, 1.0_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Dynamic,
        Layers::MOVING);

    game_object = new PhysicsModelGameObject(engine->resource_loader.load<Model>(FILE_PATH(ak47.glb)), &body_settings);
    game_object->GetBody().SetLinearVelocity(Vec3Arg(0.0f, -1.0f, 0.0f));

    floor_body.Init(&engine->physics_server, BodyCreationSettings(
        new BoxShape(Vec3Arg(8.0f, 0.1f, 8.0f)),
        RVec3Arg(0.0_r, -4.0_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Static,
        Layers::STATIC)
    );
}

void MainGameObject::Process(double deltaTime) {
    camera.Rotation.y += static_cast<float>(engine->mouseMovement.x);
    camera.Rotation.x = glm::clamp(camera.Rotation.x + static_cast<float>(engine->mouseMovement.y), glm::radians(-89.0f), glm::radians(89.0f));
    
    glm::vec4 movement(
        engine->movementInput.x * static_cast<float>(deltaTime),
        0.0f,
        engine->movementInput.y * static_cast<float>(deltaTime),
        1.0f
    );

    camera.Move(movement);
}