#include "shader_program.h"

ShaderProgram::ShaderProgram() {
	id_ = glCreateProgram();
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
	Load(vertexPath, fragmentPath);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id_);
}

void ShaderProgram::AddShader(const char* shader_code, ShaderType shader_type) {
	GLenum gl_shader_type;

	switch (shader_type)
	{
		case ShaderType::kVertex:
		gl_shader_type = GL_VERTEX_SHADER;
		break;
		case ShaderType::kFragment:
		gl_shader_type = GL_FRAGMENT_SHADER;
		break;
		default:
		break;
	}

	uint32_t shader_id = glCreateShader(gl_shader_type);
	glShaderSource(shader_id, 1, &shader_code, nullptr);
	glCompileShader(shader_id);

	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	char info_log[kInfoLogSize];
	if (!success) {
		glGetShaderInfoLog(shader_id, kInfoLogSize, NULL, info_log);
		std::cout << "SHADER:COMP:FAILED | " << shader_code << std::endl << info_log << std::endl;
	}

	glAttachShader(id_, shader_id);
	glDeleteShader(shader_id);
};

void ShaderProgram::AddShader(std::filesystem::path file_path, ShaderType shader_type) {
	std::string shader_code;
	std::ifstream shader_file;

	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shader_file.open(file_path);
		std::stringstream shader_stream;
		shader_stream << shader_file.rdbuf();
		shader_file.close();
		shader_code = shader_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ | " << file_path << std::endl;
	}

	const char* c_shader_code = shader_code.c_str();

	AddShader(c_shader_code, shader_type);
}

void ShaderProgram::Load(const char* vertexPath, const char* fragmentPath) {
	int success;
	char infoLog[kInfoLogSize];

	auto compileShader = [success, infoLog](const char* shaderPath, GLenum shaderType) mutable -> int {
		std::string shaderCode;
		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ | " << shaderPath << std::endl;
		}

		const char* cShaderCode = shaderCode.c_str();

		uint32_t shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &cShaderCode, nullptr);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shaderId, kInfoLogSize, NULL, infoLog);
			std::cout << "SHADER:COMP:FAILED | " << shaderPath << std::endl << infoLog << std::endl;
		}

		return shaderId;
	};

	uint32_t vertexId = compileShader(vertexPath, GL_VERTEX_SHADER);
	uint32_t fragmentId = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	glAttachShader(id_, vertexId);
	glAttachShader(id_, fragmentId);
	glLinkProgram(id_);

	glGetProgramiv(id_, GL_LINK_STATUS, &success);


	if (!success) {
		glGetProgramInfoLog(id_, kInfoLogSize, 0, infoLog);
		std::cout << "Program linking failed\n" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

GLint ShaderProgram::GetLocation(const char* uniform) const {
	return glGetUniformLocation(id_, uniform);
}

void ShaderProgram::Use() const {
	glUseProgram(id_);
}

int ShaderProgram::GetId() const {
	return id_;
}

void ShaderProgram::SetBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::SetInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void ShaderProgram::Link() {
	int success;
	char info_log[kInfoLogSize];

	glLinkProgram(id_);

	glGetProgramiv(id_, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(id_, kInfoLogSize, 0, info_log);
		std::cerr << "Program linking failed\n" << info_log << std::endl;
		throw std::logic_error("Program linking failed.");
	}
}

GLenum ShaderProgram::ShaderTypeToGLEnum(ShaderType shader_type) {
	switch (shader_type)
	{
		case ShaderType::kVertex:
		return GL_VERTEX_SHADER;
		break;
		case ShaderType::kFragment:
		return GL_FRAGMENT_SHADER;
		break;
		default:
		break;
	}
	
	return -1;
}
