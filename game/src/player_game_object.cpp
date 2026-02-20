#include "player_game_object.h"
#include "ball_game_object.h"

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
    ak_model = GetEngine().resource_loader.Load<Model>("ak47.glb");
    GetEngine().GetInputManager().AddAction("move", InputEventType::kKeyboard, { GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S });
    GetEngine().GetInputManager().AddCallback(this);
    GetEngine().current_camera = &camera;
	body.Init(&GetEngine().physics_server, JPH::BodyCreationSettings(
        new JPH::CapsuleShape(1.0f, 0.5f),
        JPH::RVec3Arg(0.0_r, 0.0_r, 0.0_r),
        JPH::QuatArg::sIdentity(),
        JPH::EMotionType::Dynamic,
        layers::kMoving
    ));
}

void PlayerGameObject::Process(double delta_time) {
    camera.position = RVec3tovec3(body.GetPosition());

    //ak_model->Draw(GetEngine().GetDefaultShaderProgram(),
    //    Model::GetModelMatrix(camera.position + GetFront(camera.rotation), camera.rotation + glm::vec3(0.0, -glm::pi<float>() * GetFront(camera.rotation).y, 0.0))
    //    );

    glm::vec2 movement_input = GetEngine().GetInputManager().GetAction2d("move", InputEventType::kKeyboard);

    float move_speed = 4.0f;

    JPH::Vec3 camera_front;
    
    if (noclip) {
        camera_front = euler_to_forward(camera.rotation);
    }
    else {
        auto f = GetFront(camera_rotation);
        camera_front = JPH::Vec3(f.x, f.y, f.z);
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
    vertical_velocity = JPH::max(max_fall_speed, static_cast<float>(vertical_velocity - gravity * delta_time));
}


void PlayerGameObject::ProcessInput(InputEvent& input_event) {
    if (input_event.type == InputEventType::kKeyboard
        && (input_event.press_flags == PressFlags::kJustPressed)) {
        if (input_event.key == JUMP_KEY) {
            vertical_velocity = 4.0f;
        } else if (input_event.key == GLFW_KEY_UP) {
            camera.fov -= 0.1f;
        } else if (input_event.key == GLFW_KEY_DOWN) {
            camera.fov += 0.1f;
        }
            
    } else if (input_event.type == InputEventType::kMouseButton
        && (input_event.press_flags == PressFlags::kJustPressed)
        && input_event.key == GLFW_MOUSE_BUTTON_2) {

        float length = 10.0f;

        auto start_point = JPH::Vec3Arg(camera.position.x, camera.position.y, camera.position.z);

        auto direction = GlmVecToJolt(GetFront(camera.rotation)) * length;
        auto ray_cast = JPH::RRayCast(start_point, direction);
        auto result = JPH::RayCastResult();

        auto body_ignore_filter = JPH::IgnoreSingleBodyFilter(body);
        bool hit = GetEngine().physics_server.physics_system.GetNarrowPhaseQuery().CastRay(ray_cast, result, {}, {}, body_ignore_filter);
        JPH::Vec3 end_point = hit ? (start_point + direction * result.mFraction) : start_point + direction;

        auto body_creation_setting = JPH::BodyCreationSettings(
            new JPH::SphereShape(0.25f), 
            end_point, 
            JPH::QuatArg::sIdentity(), 
            JPH::EMotionType::Dynamic, 
            layers::kMoving
        );

        Ball* ball = new Ball(GetEngine().resource_loader.Load<Model>("ball/ball.glb"), &body_creation_setting);
        GetEngine().AddGameObject(ball);
        ball->GetBody().SetRotation(JPH::QuatArg::sEulerAngles({ camera.rotation.x, -camera.rotation.y, camera.rotation.z }));
    } else if (input_event.type == InputEventType::kMouseMotion) {
        float sensitivity = 0.0035f;
        glm::vec2 mouse_movement = glm::vec2(input_event.mouse_movement_x, input_event.mouse_movement_y) * sensitivity;
        camera_rotation.y -= mouse_movement.x;
        camera_rotation.x = glm::clamp(camera.rotation.x + mouse_movement.y, glm::radians(-89.0f), glm::radians(89.0f));
        camera.rotation = camera_rotation;
    }
}