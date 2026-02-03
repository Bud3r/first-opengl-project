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

add_executable(HelloWorld "main.cpp" "shader_program.h" "texture.h" "physics_server.h" "cube.h" "camera.h" "model.h" "mesh.h" "physics_body.h")

target_link_libraries(HelloWorld PRIVATE fmt::fmt PRIVATE glad::glad PRIVATE glfw PRIVATE glm::glm PRIVATE Jolt::Jolt PRIVATE assimp::assimp)