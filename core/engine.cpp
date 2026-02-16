#include <array>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.h"

// TODO: Finish window class.
// TODO: Handle file access in release version.
// TODO: Make a game.

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

	window_ = new Window(kInitWindowWidth, kInitWindowHeight, "title");
	window_->auto_accept_quit = false;
	window_->MakeContextCurrent();
	stbi_set_flip_vertically_on_load(true);

	default_shader_program_.Load(get_real_file_path("shaders/shader.vert").c_str(), 
		get_real_file_path("shaders/shader.frag").c_str());

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

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();

	physics_server.Update((float)deltaTime);

	mouse_pos = GetInputManager().GetLastMousePos();

	double sensitivity = 0.0035;
	mouse_movement = (mouse_pos - last_mouse_pos_) * sensitivity;
	last_mouse_pos_ = mouse_pos;


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

const ShaderProgram& Engine::GetDefaultShaderProgram() const {
	return default_shader_program_;
}

void Engine::AddGameObject(GameObject* process_object) {
	process_object->engine = this;
	process_objects.push_back(process_object);
	process_object->AddedToEngine();
}

void Engine::Start() {
	double lastFrameTime = 0.0;

	while (!window_->ShouldClose())
	{
		double frameTime = glfwGetTime();
		double delta = frameTime - lastFrameTime;
		lastFrameTime = frameTime;

		if (delta > 0.0) {
			Update(delta);
		}
	}
}