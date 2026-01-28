#pragma once

#include <fmt/core.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

constexpr int InfoLogSize = 512;

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();
	void use();
	int getId();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	unsigned int ID;
};

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
	int success;
	char infoLog[InfoLogSize];

	auto compileShader = [success, infoLog](const char* shaderPath, int shaderType) mutable -> int {
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

		glGetShaderiv(shaderId, shaderType, &success);

		if (!success) {
			glGetShaderInfoLog(shaderId, InfoLogSize, NULL, infoLog);
			std::cout << "SHADER:COMP:FAILED | " << shaderPath << std::endl << infoLog << std::endl;
		}
		
		return shaderId;
		};

	uint32_t vertexId = compileShader(vertexPath, GL_VERTEX_SHADER);
	uint32_t fragmentId = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	
 	ID = glCreateProgram();
	glAttachShader(ID, vertexId);
	glAttachShader(ID, fragmentId);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, InfoLogSize, 0, infoLog);
		std::cout << "Program linking failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(ID);
}

void ShaderProgram::use() {
	glUseProgram(ID);
}

int ShaderProgram::getId() {
	return ID;
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}