cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Jolt CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(HelloWorld "main.cpp" "game_object.cpp" "camera.cpp" "main_game_object.cpp" "physics_model_game_object.cpp" "shader_program.h" "texture.h" "physics_server.h"  "camera.h" "model.h" "mesh.h" "physics_body.h" "engine.h" "engine.cpp" "game_object.h" "resource_loader.h" "main_game_object.h" "physics_model_game_object.h" "mesh.cpp" "model.cpp" "physics_body.cpp" "physics_server.cpp" "resource_loader.cpp" "shader_program.cpp" "texture.cpp")

target_link_libraries(HelloWorld PRIVATE fmt::fmt PRIVATE glad::glad PRIVATE glfw PRIVATE glm::glm PRIVATE Jolt::Jolt PRIVATE assimp::assimp)