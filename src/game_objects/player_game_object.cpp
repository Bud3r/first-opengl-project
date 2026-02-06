#include "player_game_object.h"


constexpr int JUMP_KEY = GLFW_KEY_SPACE;


void PlayerGameObject::AddedToEngine() {
    Input_add_callback(this);
    get_engine().current_camera = &camera;
	body.Init(&get_engine().physics_server, BodyCreationSettings(
        new BoxShape(Vec3Arg(0.2f, 0.2f, 0.2f)),
        RVec3Arg(0.0_r, 0.0_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Dynamic,
        Layers::MOVING
    ));
}

void PlayerGameObject::Process(double delta_time) {
    camera.Rotation.y += static_cast<float>(get_engine().mouseMovement.x);
    camera.Rotation.x = glm::clamp(camera.Rotation.x + static_cast<float>(get_engine().mouseMovement.y), glm::radians(-89.0f), glm::radians(89.0f));
    camera.Position = RVec3tovec3(body.GetPosition());

    glm::vec2 movement_input = Input_get_action_2d("move", InputEventType::KEYBOARD);

    float move_speed = 1.0f;

    JPH::Vec3 camera_front;
    
    if (noclip) {
        camera_front = JPH::Vec3(
            cos(camera.Rotation.y) * cos(camera.Rotation.x),
            -sin(camera.Rotation.x),
            sin(camera.Rotation.y) * cos(camera.Rotation.x)
        ).Normalized();
    }
    else {
        camera_front = JPH::Vec3(
            sin(camera.Rotation.y),
            0.0,
            cos(camera.Rotation.y)
        ).Normalized();
    }
    
    JPH::Vec3 camera_right = up.Cross(camera_front);

    JPH::Vec3 movement_dir = (camera_front * movement_input.y + camera_right * movement_input.x) * move_speed;

    JPH::Vec3 velocity;

    if (noclip) {
        velocity = movement_dir;
    }
    else {
        velocity = JPH::Vec3(
            movement_dir.GetX(),
            vertical_velocity,
            movement_dir.GetZ()
        );
    }

    body.SetLinearVelocity(velocity);

    float gravity = 1.0f;
    float max_fall_speed = -2.0f;
    vertical_velocity = max(max_fall_speed, static_cast<float>(vertical_velocity - gravity * delta_time));
    
    printf("Pos: ");
    print_vec(RVec3tovec3(body.GetPosition()));
}


void PlayerGameObject::process_input(InputEvent& input_event) {
    if (input_event.m_type == InputEventType::KEYBOARD
        && input_event.m_key_label == JUMP_KEY
        && (input_event.m_press_flags == PressFlags::JUST_PRESSED)) {
        vertical_velocity = 1.0f;
    }
}