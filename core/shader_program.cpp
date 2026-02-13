#include "shader_program.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
	Load(vertexPath, fragmentPath);
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


	id_ = glCreateProgram();
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

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id_);
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