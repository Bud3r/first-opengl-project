cmake_minimum_required(VERSION 3.10)

# Can use this to make building cleaner.
#"${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
set(assets_path "${CMAKE_CURRENT_SOURCE_DIR}/game/assets/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(DEBUG_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/game/assets/")
add_compile_definitions(COMPILE_ASSETS_PATH="./assets/")

project(BudderEngine)

find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Jolt CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(
	BudderEngine-Core
	STATIC
	"core/helper.h" "core/helper.cpp"
	"core/camera.cpp" "core/camera.h"
	"core/engine.cpp" "core/engine.h"
	"core/game_object.cpp" "core/game_object.h"
	"core/input_manager.cpp" "core/input_manager.h"
	"core/mesh.cpp" "core/mesh.h"
	"core/model.cpp" "core/model.h"
	"core/physics_body.cpp" "core/physics_body.h"
	"core/physics_server.cpp" "core/physics_server.h"
	"core/resource_loader.cpp" "core/resource_loader.h"
	"core/shader_program.cpp" "core/shader_program.h"
	"core/texture.cpp" "core/texture.h"
	"core/window.cpp" "core/window.h"
	"core/resource_loaders/model_loader.h" "core/resource_loaders/model_loader.cpp"
	"core/resource_loaders/texture_loader.h" "core/resource_loaders/texture_loader.cpp"
	"core/resource_loaders/resource_format_loader.h"
     "core/resource_loaders/shader_program_loader.h" "core/resource_loaders/shader_program_loader.cpp")

target_include_directories(BudderEngine-Core PUBLIC "core/include")

target_link_libraries(BudderEngine-Core
	PUBLIC fmt::fmt 
	PUBLIC glad::glad 
	PUBLIC glfw 
	PUBLIC glm::glm 
	PUBLIC Jolt::Jolt 
	PUBLIC assimp::assimp 
	PUBLIC imgui::imgui
)

# - You can change the name "BudderEngine-Game" to anything you want.
add_executable(BudderEngine-Game
	"game/src/main.cpp"
	"game/src/player_game_object.cpp" "game/src/player_game_object.h"
	"game/src/main_game_object.cpp" "game/src/main_game_object.h"
	"game/src/physics_model_game_object.cpp" "game/src/physics_model_game_object.h"
	"game/src/ball_game_object.cpp"
)

target_include_directories(BudderEngine-Game 
	PRIVATE 
	"core"
)

target_link_libraries(BudderEngine-Game
	PRIVATE BudderEngine-Core
)

#add_custom_command(
#	TARGET BudderEngine-Game POST_BUILD
#	COMMAND "${CMAKE_BINARY_DIR}/bin/BudderEngine-Game.exe" "-pack" "${CMAKE_CURRENT_SOURCE_DIR}/game/assets/"
#	VERBATIM
#)

set_target_properties(
  BudderEngine-Game
  PROPERTIES
  DEBUG_POSTFIX "-Debug"
)