cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(HelloWorld "main.cpp" "shader_program.h" "texture.h")

target_link_libraries(HelloWorld PRIVATE fmt::fmt PRIVATE glad::glad PRIVATE glfw PRIVATE glm::glm)