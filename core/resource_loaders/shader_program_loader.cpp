#include "shader_program_loader.h"


void* ShaderProgramLoader::LoadFromFile(std::string file_path) {
	std::ifstream file(file_path);
	return LoadFromStream(file, file_path);
}

void* ShaderProgramLoader::LoadFromMemory(void* ptr, uintmax_t len) {
	char* c_string = reinterpret_cast<char*>(ptr);
	std::string file_string(c_string);
	std::istringstream stream(file_string);
	return LoadFromStream(stream);
}

void* ShaderProgramLoader::LoadFromStream(std::istream& stream, std::filesystem::path file_path) {
	const int kMaxShaderSize = 4096;
	char shader_code[kMaxShaderSize] = { };

	ShaderProgram* program = new ShaderProgram();

	while (stream.getline(shader_code, kMaxShaderSize, '=')) {
		ShaderProgram::ShaderType shader_type;

		if (!std::strcmp(shader_code, "frag")) {
			shader_type = ShaderProgram::ShaderType::kFragment;
		} else if (!std::strcmp(shader_code, "vert")) {
			shader_type = ShaderProgram::ShaderType::kVertex;
		} else {
			throw std::logic_error("Invalid shader type.");
		}

		if (stream.get() == '"') {
			stream.getline(shader_code, kMaxShaderSize, '"');
			program->AddShader(shader_code, shader_type);
		} else {
			stream.unget();
			stream.getline(shader_code, kMaxShaderSize);
			program->AddShader(file_path.replace_filename(shader_code), shader_type);
		}
	}

	program->Link();

	return program;
}

std::vector<std::string> ShaderProgramLoader::GetSupportedExtensions() {
	return { ".shader" };
}

void ShaderProgramLoader::CreateGeneratedAsset(
	std::ifstream& base_file, 
	std::ofstream& asset_file, 
	std::filesystem::path base_file_path) {

	const int kMaxShaderSize = 4096;
	char shader_code[kMaxShaderSize] = { };

	while (base_file.getline(shader_code, kMaxShaderSize, '=')) {
		std::string type;

		if (!std::strcmp(shader_code, "frag") || !std::strcmp(shader_code, "vert")) {
			type = shader_code;
			asset_file << type << "=";
		}
		else {
			std::cerr << "Invalid shader type." << std::endl;
			throw std::logic_error("Invalid shader type.");
		}

		if (base_file.get() == '"') {
			base_file.getline(shader_code, kMaxShaderSize, '"');
			asset_file << '"' << shader_code << '"';
		}
		else {
			base_file.unget();
			base_file.getline(shader_code, kMaxShaderSize);
			std::ifstream shader_file(base_file_path.replace_filename(shader_code));
			std::cout << shader_code << std::endl;
			asset_file << '"' << shader_file.rdbuf() << '"';
			shader_file.close();
		}

		asset_file << "\n";
	}
}
