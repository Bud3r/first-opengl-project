#include "main_game_object.h"

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


void MainGameObject::AddedToEngine() {
    get_engine().physics_server.m_physics_system.SetGravity(Vec3Arg(0.0, -0.1, 0.0));

    auto ak_body_settings = BodyCreationSettings(
        new BoxShape(Vec3Arg(1.0f, 1.0f, 1.0f)),
        RVec3Arg(0.0_r, 0.5_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Dynamic,
        Layers::MOVING);

    auto floor_body_settings = BodyCreationSettings(
        new BoxShape(Vec3Arg(12.0f, 0.1f, 12.0f)),
        RVec3Arg(0.0_r, -1.0_r, 0.0_r),
        QuatArg::sIdentity(),
        EMotionType::Static,
        Layers::STATIC
        );

    Model* ak_model = get_engine().resource_loader.load<Model>(FILE_PATH(ak47.glb));

    Mesh* floor_mesh = _create_square_mesh(glm::vec3(8.0f, 0.1f, 8.0f));
    Model* floor_model = new Model(floor_mesh);
    floor_game_object = new PhysicsModelGameObject(floor_model, &floor_body_settings);
    get_engine().add_game_object(floor_game_object);
    floor_game_object->modulate = glm::vec4(0.5, 0.5, 0.5, 1.0);

    ak_game_object = new PhysicsModelGameObject(ak_model, &ak_body_settings);
    get_engine().add_game_object(ak_game_object);
    //ak_game_object->GetBody().SetLinearVelocity(Vec3Arg(0.0f, -1.0f, 0.0f));

    player_game_object = new PlayerGameObject();
    get_engine().add_game_object(player_game_object);
}

