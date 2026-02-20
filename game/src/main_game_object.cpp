#include "main_game_object.h"

namespace {
Mesh* _create_square_mesh(glm::vec3 e) {

    std::vector<Vertex> vertices = {};
    std::vector<uint32_t> indicices = {
        1, 2, 3,
        2, 3, 4,
        2, 3, 6,
        2, 6, 7,
        1, 2, 7,
        1, 2, 8,
        1, 4, 5,
        5, 8, 1,
        3, 4, 6,
        5, 6, 4,
        1, 1, 1,
        1, 1, 1
    };

    vertices.push_back(Vertex{ glm::vec3(e.x, e.y, e.z) });
    vertices.push_back(Vertex{ glm::vec3(-e.x, e.y, e.z) });
    vertices.push_back(Vertex{ glm::vec3(-e.x, -e.y, e.z) });
    vertices.push_back(Vertex{ glm::vec3(e.x, e.y, -e.z) });
    vertices.push_back(Vertex{ glm::vec3(e.x, -e.y, -e.z) });
    vertices.push_back(Vertex{ glm::vec3(-e.x, e.y, -e.z) });
    vertices.push_back(Vertex{ glm::vec3(e.x, -e.y, e.z) });
    vertices.push_back(Vertex{ glm::vec3(e.x, -e.y, e.z) });

    Mesh* floor_mesh = new Mesh(vertices, indicices, {});
    return floor_mesh;
}
} // namespace


void MainGameObject::AddedToEngine() {
    auto ak_body_settings = JPH::BodyCreationSettings(
        new JPH::BoxShape(JPH::Vec3Arg(1.0f, 1.0f, 1.0f)),
        JPH::RVec3Arg(0.0_r, 0.5_r, 0.0_r),
        JPH::QuatArg::sIdentity(),
        JPH::EMotionType::Dynamic,
        layers::kMoving);

    auto floor_body_settings = JPH::BodyCreationSettings(
        new JPH::BoxShape(JPH::Vec3Arg(12.0f, 0.1f, 12.0f)),
        JPH::RVec3Arg(0.0_r, -1.0_r, 0.0_r),
        JPH::QuatArg::sIdentity(),
        JPH::EMotionType::Static,
        layers::kStatic
        );

    std::shared_ptr<Model> ak_model = GetEngine().resource_loader.Load<Model>("ak47.glb");

    Mesh* floor_mesh = _create_square_mesh(glm::vec3(8.0f, 0.1f, 8.0f));
    std::shared_ptr<Model> floor_model = std::make_shared<Model>(floor_mesh);
    floor_game_object = new PhysicsModelGameObject(floor_model, &floor_body_settings);
    GetEngine().AddGameObject(floor_game_object);
    floor_game_object->modulate = glm::vec4(0.5, 0.5, 0.5, 1.0);

    //ak_game_object = new PhysicsModelGameObject(ak_model, &ak_body_settings);
    //GetEngine().AddGameObject(ak_game_object);
    //ak_game_object->GetBody().SetLinearVelocity(Vec3Arg(0.0f, -1.0f, 0.0f));

    player_game_object = new PlayerGameObject();
    GetEngine().AddGameObject(player_game_object);

    model = ak_model;
}

void MainGameObject::Process(double delta) {
    glm::mat4 model_mat(1.0f);
    model_mat = glm::translate(model_mat, { 0.0f, -0.25f , 1.0f });
    model->Draw(GetEngine().GetDefaultShaderProgram(), model_mat);
}

