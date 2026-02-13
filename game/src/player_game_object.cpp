#include "player_game_object.h"
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

constexpr int JUMP_KEY = GLFW_KEY_SPACE;

inline JPH::Vec3 euler_to_forward(glm::vec3 euler) {
    return JPH::Vec3(
        sin(euler.y),
        0.0f,
        cos(euler.y)
    ).Normalized();
}


void PlayerGameObject::AddedToEngine() {
    Input_add_action("move", InputEventType::KEYBOARD, { GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S });
    Input_add_callback(this);
    get_engine().current_camera = &camera;
	body.Init(&get_engine().physics_server, BodyCreationSettings(
        new CapsuleShape(1.0f, 0.5f),
        RVec3Arg(0.0_r, 0.0_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Dynamic,
        Layers::MOVING
    ));
}

void PlayerGameObject::Process(double delta_time) {
    camera_rotation.y += static_cast<float>(get_engine().mouseMovement.x);
    camera_rotation.x = glm::clamp(camera.Rotation.x + static_cast<float>(get_engine().mouseMovement.y), glm::radians(-89.0f), glm::radians(89.0f));
    camera.Rotation = camera_rotation;
    camera.Position = RVec3tovec3(body.GetPosition());

    glm::vec2 movement_input = Input_get_action_2d("move", InputEventType::KEYBOARD);

    float move_speed = 4.0f;

    JPH::Vec3 camera_front;
    
    if (noclip) {
        camera_front = euler_to_forward(camera.Rotation);
    }
    else {
        camera_front = JPH::Vec3(
            cos(camera.Rotation.y),
            0.0f,
            sin(camera.Rotation.y)
        ).Normalized();
    }
    
    
    JPH::Vec3 camera_right = camera_front.Cross(up).Normalized();

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

    float gravity = 9.8f;
    float max_fall_speed = -2.0f;
    vertical_velocity = max(max_fall_speed, static_cast<float>(vertical_velocity - gravity * delta_time));
}


void PlayerGameObject::process_input(InputEvent& input_event) {
    if (input_event.m_type == InputEventType::KEYBOARD
        && input_event.m_key_label == JUMP_KEY
        && (input_event.m_press_flags == PressFlags::JUST_PRESSED)) {
        vertical_velocity = 4.0f;
    }
    if (input_event.m_type == InputEventType::KEYBOARD
        && input_event.m_key_label == GLFW_KEY_Q
        && (input_event.m_press_flags == PressFlags::JUST_PRESSED)) {
        
        float length = 5.0f;
        auto start_point = Vec3Arg(camera.Position.x, camera.Position.y, camera.Position.z);
        auto direction = euler_to_forward(camera.Rotation) * length;
        auto ray_cast = JPH::RRayCast(start_point, direction);
        auto result = JPH::RayCastResult();
        auto body_ignore_filter = JPH::IgnoreSingleBodyFilter(body);
        bool hit = get_engine().physics_server.m_physics_system.GetNarrowPhaseQuery().CastRay(ray_cast, result, {}, {}, body_ignore_filter);
        Vec3 end_point = hit ? (start_point + direction * result.mFraction) : start_point + direction;
        printf("Start: ");
        print_vec(Vec3tovec3(start_point));
        printf("Dir:");
        print_vec(Vec3tovec3(direction));
        auto body_creation_setting = BodyCreationSettings(new SphereShape(0.5f), end_point, QuatArg::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
        PhysicsModelGameObject* object = new PhysicsModelGameObject(get_engine().resource_loader.load<Model>("ball/ball.glb").get(), &body_creation_setting);
        get_engine().add_game_object(object);
    }
}