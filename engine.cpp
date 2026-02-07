#include "engine.h"


void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool _show_demo_window = true;


Engine::Engine() {
	//float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	m_window = CreateWindow();
	default_shader_program.Load(FILE_PATH(shader.vert), FILE_PATH(shader.frag));

	glfwGetCursorPos(m_window, &lastMousePos.x, &lastMousePos.y);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, UBO_BINDING, UBO, 0, 2 * sizeof(glm::mat4));

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(1.0f);

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	
	ImGui_ImplOpenGL3_Init("#version 450");
}

Engine::~Engine() {
	glfwDestroyWindow(m_window);
}

void Engine::Update(double deltaTime)
{
	glfwMakeContextCurrent(m_window);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	physics_server.Update((float)deltaTime);

	glfwGetCursorPos(m_window, &mousePos.x, &mousePos.y);

	double sensitivity = 0.0035;
	mouseMovement = (mousePos - lastMousePos) * sensitivity;
	lastMousePos = mousePos;

	if (deltaTime > 0.0) {
		glfwSetWindowTitle(m_window, (std::string("FPS: ") + std::to_string(static_cast<int>(1.0 / deltaTime))).c_str());
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (current_camera != nullptr) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);

		glm::mat4 view = current_camera->GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));

		glm::mat4 projection = current_camera->GetProjectionMatrix(static_cast<float>(INIT_WINDOW_WIDTH) / static_cast<float>(INIT_WINDOW_HEIGHT));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	if (_show_demo_window){
		ImGui::ShowDemoWindow(&_show_demo_window);
	}

	for (auto obj : process_objects) {
		obj->Process(deltaTime);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Engine::Setup()
{
	SetupJolt();
	glfwInit();
}

void Engine::Dismantle()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	DismantleJolt();
}

const ShaderProgram& Engine::GetDefaultShaderProgram() const {
	return default_shader_program;
}

void Engine::add_game_object(GameObject* process_object) {
	process_object->engine = this;
	process_objects.push_back(process_object);
	process_object->AddedToEngine();
}

GLFWwindow* Engine::CreateWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, "Title", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fmt::println("Failed to initialize GLAD");
		// TODO: May need to return error or throw exeption.
		return nullptr;
	}
	glViewport(0, 0, INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);
	Input_init(window);
	return window;
}