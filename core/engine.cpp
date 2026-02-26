#include <array>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.h"

// TODO: Finish window class.
// TODO: Handle file access in release version.
// TODO: Make a game.

std::filesystem::path Engine::executable_path = std::filesystem::path("");
FMOD::Studio::System* Engine::fmod_system = nullptr;

namespace {
	int width_ = kInitWindowWidth;
	int height_ = kInitWindowHeight;
	bool _show_demo_window = true;
	
	void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		width_ = width;
		height_ = height;
	}
} // namespace


Engine::Engine() {
	glfwInit();
	
	const int kFmodChannelCount = 32;
	FMOD::Studio::System::create(&fmod_system);
	fmod_system->initialize(
		kFmodChannelCount,
		FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE,
		0,
		nullptr
	);

	window_ = new Window(kInitWindowWidth, kInitWindowHeight, "title");
	window_->auto_accept_quit = false;
	window_->MakeContextCurrent();
	stbi_set_flip_vertically_on_load(true);
	unsigned char default_texture_data[3 * 4] = {
		255, 0, 255,
		0, 0, 0,
		255, 0, 255,
		0, 0, 0
	};

	default_texture_ = std::shared_ptr<Texture>(Texture::FromData(default_texture_data, 2, 2, 3));

	glGenBuffers(1, &ubo_);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, kUboBinding, ubo_, 0, 2 * sizeof(glm::mat4));

	glEnable(GL_DEPTH_TEST);

	last_mouse_pos_ = GetInputManager().GetLastMousePos();
	GetInputManager().SetMouseMode(MouseMode::kDisabled);
	GetInputManager().SetRawMouseMotion(true);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(1.0f);
	ImGui_ImplGlfw_InitForOpenGL(window_->GetGlfwWindow(), true);
	
	ImGui_ImplOpenGL3_Init("#version 450");
}

Engine::~Engine() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Engine::Update(double deltaTime)
{
	glfwMakeContextCurrent(window_->GetGlfwWindow());

	//fmod_system->setListenerAttributes(0, FMOD_3D_ATTRIBUTES::position, { 0.0f, 0.0f, 0.0f })

	fmod_system->update();
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();https://open.spotify.com/user/e0bm35lelv4s9l99zm9cfe27i/collection
	//ImGui::NewFrame();

	physics_server.Update((float)deltaTime);

	if (deltaTime > 0.0) {
		int fps = static_cast<int>(1.0 / deltaTime);
		window_->SetTitle((std::string("FPS: ") + std::to_string(fps)).c_str());
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (current_camera != nullptr) {
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);

		glm::mat4 view = current_camera->GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glm::mat4 projection = current_camera->GetProjectionMatrix(static_cast<float>(width_) / static_cast<float>(height_));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		FMOD_3D_ATTRIBUTES listener_attribs{
		FMOD_VECTOR { current_camera->position.x, current_camera->position.y, current_camera->position.z },
		FMOD_VECTOR { 0.0f, 0.0f, 0.0f },
		FMOD_VECTOR { 0.0f, 0.0f, 1.0f },
		FMOD_VECTOR { 0.0f, 1.0f, 0.0f },
		};
		fmod_system->setListenerAttributes(0, &listener_attribs);
	}

	//if (_show_demo_window){
	//	ImGui::ShowDemoWindow(&_show_demo_window);
	//	_show_demo_window = false;
	//}


	for (auto obj : process_objects) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, default_texture_->GetId());
		obj->Process(deltaTime);
	}

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window_->GetGlfwWindow());
	glfwPollEvents();
}

void Engine::SetCmdLineArguments(int argc, char* args[]) {
	if (argc < 1) {
		throw std::logic_error("No arguments, can't get path.");
		return;
	}

	executable_path = args[0];
	
	if (argc < 2) {
		return;
	}
	
	if (std::strcmp(args[1], "-pack") == 0) {
		if (argc != 3) {
			throw std::logic_error("Wrong argument count for the '-pack' command.");
		}

		resource_loader.SaveAssetPack(args[2], executable_path.replace_extension(".pck"));
		Close();
	} else {
		throw std::logic_error("Unknown command.");
	}
}

const std::shared_ptr<ShaderProgram> Engine::GetDefaultShaderProgram() const {
	return default_shader_program_;
}

void Engine::AddGameObject(GameObject* process_object) {
	process_object->engine = this;
	process_objects.push_back(process_object);
	process_object->AddedToEngine();
}