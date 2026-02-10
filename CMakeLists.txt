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
find_package(imgui CONFIG REQUIRED)

add_executable(HelloWorld 
	"src/main.cpp" 
	"src/mesh.cpp" 
	"src/model.cpp" 
	"src/physics_body.cpp" 
	"src/physics_server.cpp"
	"src/resource_loader.cpp" 
	"src/game_object.cpp" 
	"src/camera.cpp" 
	"src/main_game_object.cpp" 
	"src/physics_model_game_object.cpp" 
	"src/shader_program.cpp" 
	"src/texture.cpp" 
	"src/input_manager.cpp" 
	"src/game_objects/player_game_object.cpp"
	"src/engine.cpp"
)
	#"src/input_manager.h" 
	#"src/game_objects/player_game_object.h" 
	#"src/main_game_object.h"
	#"src/shader_program.h" 
	#"src/texture.h" 
	#"src/physics_server.h"  
	#"src/camera.h" 
	#"src/model.h" 
	#"src/mesh.h" 
	#"src/physics_body.h" 
	#"src/engine.h"
	#"src/game_object.h"
	#"src/resource_loader.h" 
	#"src/physics_model_game_object.h" 
	
target_include_directories(HelloWorld PRIVATE "include")

target_link_libraries(HelloWorld 
	PRIVATE fmt::fmt 
	PRIVATE glad::glad 
	PRIVATE glfw 
	PRIVATE glm::glm 
	PRIVATE Jolt::Jolt 
	PRIVATE assimp::assimp 
	PRIVATE imgui::imgui
)